#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxTrueTypeFontUC.h"
#include "WindowManager.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxEtherdream.h"


#define LASER_WIDTH 400
#define LASER_HEIGHT 400

using namespace cv;
using namespace ofxCv;

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    void mainCommentFBODraw();
    wstring stringConvertToW(const char* c_buff);
    
    void serialComDraw();
    void exit();
    
    /* Socket Connection */
    ofxUDPManager udpConnection;
    
    /* 日本語フォント */
    ofxTrueTypeFontUC commentFont;
    ofTrueTypeFont commentFont2;
    
    /* コメント表示関係の変数 */
    vector<wstring> comments;
    int commentCounter;
    wstring tempMessage0;
    wstring tempMessage1;
    wstring tempMessage2;
    wstring tempMessage3;
    wstring tempMessage4;
    wstring tempMessage5;
    wstring tempMessage6;
    wstring tempMessage7;
    wstring tempMessage8;
    wstring tempMessage9;
    
    ofPoint tempMessagePos0;
    ofPoint tempMessagePos1;
    ofPoint tempMessagePos2;
    ofPoint tempMessagePos3;
    ofPoint tempMessagePos4;
    ofPoint tempMessagePos5;
    ofPoint tempMessagePos6;
    ofPoint tempMessagePos7;
    ofPoint tempMessagePos8;
    ofPoint tempMessagePos9;

    /* debug variables */
    vector<ofPoint> debugPos;
    
    /* for Laser Image */
    ofImage grabImage;
    ofImage cannyImage;
    ofImage blurImage;
    ofxCvColorImage ofxDstVid;
    
    /* fbo */
    ofFbo			myFbo;    
    
    /* effect */
    ofEasyCam cam;
    ofLight light;
    
    /* MultiWindow */
    WindowManager winManager;
    
    /* GUI */
    ofxPanel imageGui;
    ofxFloatSlider blurGUI;
    ofxVec2Slider cannyThresholdGUI;
    ofxVec2Slider renderAdaptiveThreashold;
    ofxVec2Slider renderAdaptiveThreashold2;
    ofxFloatSlider smoothAmountGUI;
    ofxFloatSlider optimizeToleranceGUI;
    ofxFloatSlider collapseGUI;
    ofxFloatSlider spacingGUI;
    
    /* LASER */
    ofxIlda::Frame ildaFrame;
    ofxIlda::RenderTarget ildaRender;
    ofxEtherdream etherdream;
    ofPoint grabDispPos;
};
