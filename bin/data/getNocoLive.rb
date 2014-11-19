# -*- encoding: utf-8 -*-
require 'net/http'
require 'nokogiri'
require "socket"


class Comment
  class << self
    attr_accessor :cookie
  end
  #下の値はブラウザのクッキーから取得してください
  self.cookie = 'user_session=user_session_24138916_95ccd75174f666c59f03a7c7a4ff6559a75789d49cfbe4d993553400707f8bf1'
  def initialize(live_id)
    @live_id = live_id
    @status = get_playerstatus()

    @udp = UDPSocket.open()
    @sockaddr = Socket.pack_sockaddr_in(10000, "127.0.0.1")
    @sockaddr_2 = Socket.pack_sockaddr_in(10000, "192.168.1.48")
    @sockaddr_3 = Socket.pack_sockaddr_in(10000, "192.168.1.39")
    @commentCount = 0
    @deviceCount = 3
  end

  def get_playerstatus
    Net::HTTP.start 'watch.live.nicovideo.jp' do |http|
      response = http.get "/api/getplayerstatus?v=#{@live_id}",
            {'Cookie' => Comment.cookie, 'User-Agent' => 'Ruby/Nicolive'}
      xml = Nokogiri(response.body)
      puts xml
      # if not xml.elements.attribute('status').value() == 'ok'
      #   raise '放送は既に終了しています'
      # end
      {
        address: (xml/:addr).inner_text(),
        port: (xml/:port).inner_text(),
        thread: (xml/:thread).inner_text()
      }
    end
  end
  def start()
    TCPSocket.open(@status[:address], @status[:port].to_i) do |sock|
    #TCPSocket.open("127.0.0.1", "20000") do |sock|
      ticket = "<thread thread=\"#{@status[:thread]}\" version=\"20061206\" res_from=\"-100\"/>\0"
      sock.write(ticket)
      p ticket
      while true
        stream = sock.gets("\0")
        xml = Nokogiri(stream)
        comment = (xml/:chat).inner_text()
        
        if comment != "" && comment.include?("/") != true then
          puts comment + "\0"

          if @commentCount % @deviceCount == 0 then
            @udp.send(comment, 0, @sockaddr)
            @commentCount += 1
          elsif @commentCount % @deviceCount == 1 then            
            @udp.send(comment, 0, @sockaddr_2)
            @commentCount += 1
          elsif @commentCount % @deviceCount == 2 then
            @udp.send(comment, 0, @sockaddr_3)
            @commentCount += 1
          end
        end
      end
    end
    # while true do
    #   sleep 1
    #   t = Time.now
    #   time = str = t.strftime("現在時刻 %H時 %M分 %S秒")
    #   puts time
    #   @udp.send(time, 0, @sockaddr)
    # end
  end
end

#みたい放送の放送IDを入れる(coxxxxxxx形式でもOK)
comment = Comment.new 'lv200169195'
comment.start()
