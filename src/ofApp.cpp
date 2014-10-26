#include "ofApp.h"

bool showStr;


wstring ofApp::stringConvertToW(const char* c_buff) {
    setlocale( LC_ALL, "ja_JP.UTF-8" );
    const char*   c = c_buff;
    size_t     length = strlen( c );
    wchar_t*   wc = new wchar_t[ length + 1 ];
    length = mbstowcs( wc, c, length + 1);
    return wc;
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    /* basic settings */
    ofSetFrameRate(30);
    ofBackground(0, 0, 0);
    ofSetVerticalSync(30);
    
    /* gui */
    imageGui.setup();
    imageGui.add(blurGUI.setup( "blurGUI", 10, 0, 100 ));
    imageGui.add(cannyThresholdGUI.setup("cannyThresholdGUI",ofVec2f(100, 50),ofVec2f(0,0),ofVec2f(255,255)));
    imageGui.add(renderAdaptiveThreashold.setup("renderAdaptiveThreashold",ofVec2f(0, 0),ofVec2f(0,0),ofVec2f(255,255)));
    imageGui.add(renderAdaptiveThreashold2.setup("renderAdaptiveThreashold2",ofVec2f(0, 0),ofVec2f(0,0),ofVec2f(255,255)));
    
    
    /* UDP Connection */
    udpConnection.Create();
    udpConnection.Bind(10000);
    udpConnection.SetNonBlocking(true);
    
    /* ofxTruetypeFont */
    commentFont.loadFont("ヒラギノ角ゴ Pro W6.otf", 70, true, true);
    
    /* コメント管理 */
    commentCounter = 0;
    
    /* laserImageProcessing */
    grabImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    blurImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    cannyImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    ofxDstVid.allocate(ofGetWidth(), ofGetHeight());
    
    /* effect */
    myFbo.allocate(ofGetWidth(), ofGetHeight());
    
    // Setup light
	//light.setPosition(1000, 1000, 2000);
    
    /* multi Windows */
    winManager.setup((ofxAppGLFWWindowMulti *)ofGetWindowPtr());
    winManager.loadWindowSettings();
    
    /* LASER */
    etherdream.setup();
    etherdream.setPPS(30000);
    
    /* Render */
    ildaRender.setup(ofGetWidth(), ofGetHeight());
    
    /* initialize */
    string hogemess = "おはよう";
    ofRectangle rect;
    rect = commentFont.getStringBoundingBox(stringConvertToW(hogemess.c_str()), 0, 0);
    float stringWidth = rect.width;
    float stringHeight = rect.height;
    
    tempMessage9 = stringConvertToW(hogemess.c_str());
    tempMessagePos9 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    
    showStr = true;

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    cannyImage.update();
    blurImage.update();
    
    
    char udpMessage[100000];
    udpConnection.Receive(udpMessage,100);
    string message=udpMessage;
    if(message!=""){
        cout << message << endl;
        serialComUpdate(message);
    }
    
    /* 画像処理 */
    /* ぼかし */
    Mat srcBlur, preBlur;
    blur(grabImage, preBlur, blurGUI);
    toOf(preBlur, blurImage);
    
    /* 輪郭抽出 */
    Mat preConvertImage, preCannyImage;
    ofImage srcCannyImage;
    srcCannyImage.setFromPixels(blurImage.getPixels(), ofGetWidth(), ofGetHeight(),  OF_IMAGE_COLOR);
    
    convertColor(srcCannyImage, preConvertImage, CV_RGB2GRAY);
    Canny(preConvertImage, preCannyImage, cannyThresholdGUI->x, cannyThresholdGUI->y, 3);
    toOf(preCannyImage, cannyImage);
    
    /* FBO */
    mainCommentFBODraw();
    
//    /* renderTarget params */
//    ildaRender.params.cv.adaptiveThresholdAmount = renderAdaptiveThreashold->x;
//    ildaRender.params.cv.adaptiveThresholdBlock = renderAdaptiveThreashold->y;
    

}

void ofApp::serialComUpdate(string _message) {
    ofRectangle rect;
    rect = commentFont.getStringBoundingBox(stringConvertToW(_message.c_str()), 0, 0);
    float stringWidth = rect.width;
    float stringHeight = rect.height;
    
    if (commentCounter == 0) {
        tempMessage0 = stringConvertToW(_message.c_str());
        tempMessagePos0 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    } else if (commentCounter == 1) {
        tempMessage1 = stringConvertToW(_message.c_str());
        tempMessagePos1 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    } else if (commentCounter == 2){
        tempMessage2 = stringConvertToW(_message.c_str());
        tempMessagePos2 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    } else if (commentCounter == 3) {
        tempMessage3 = stringConvertToW(_message.c_str());
        tempMessagePos3 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    } else if (commentCounter == 4) {
        tempMessage4 = stringConvertToW(_message.c_str());
        tempMessagePos4 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    } else if (commentCounter == 5) {
        tempMessage5 = stringConvertToW(_message.c_str());
        tempMessagePos5 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    } else if (commentCounter == 6) {
        tempMessage6 = stringConvertToW(_message.c_str());
        tempMessagePos6 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    } else if (commentCounter == 7) {
        tempMessage7 = stringConvertToW(_message.c_str());
        tempMessagePos7 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    } else if (commentCounter == 8) {
        tempMessage8 = stringConvertToW(_message.c_str());
        tempMessagePos8 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    } else if (commentCounter == 9) {
        tempMessage9 = stringConvertToW(_message.c_str());
        tempMessagePos9 = ofPoint(ofRandom(0, ofGetWidth() - stringWidth), ofRandom(stringHeight, ofGetHeight() - stringHeight));
    } else {
        commentCounter = 0;
    }
    
    commentCounter++;
    //debugPos.clear();
    
    debugPos.push_back(tempMessagePos0);
    debugPos.push_back(tempMessagePos1);
    debugPos.push_back(tempMessagePos2);
    debugPos.push_back(tempMessagePos3);
    debugPos.push_back(tempMessagePos4);
    debugPos.push_back(tempMessagePos5);
    debugPos.push_back(tempMessagePos6);
    debugPos.push_back(tempMessagePos7);
    debugPos.push_back(tempMessagePos8);
    debugPos.push_back(tempMessagePos9);
    
    for (int i=0; i < debugPos.size(); i++) {
        for (int j=0; j < debugPos.size(); j++) {
            
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255, 255, 255);
    if( winManager.getActiveWindowNo() == 0){
        
        //myFbo.draw(0, 0);
        
        
        ofClear(0, 0, 0,255);
        ofBackground(0, 0, 0);
        ofPushMatrix();
        ofSetColor(255, 0, 0);
        ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
        ofRotateZ(ofGetFrameNum());
        //ofRect(- 50, -50, 100, 100);
        ofPopMatrix();
        if(showStr) serialComDraw();
        
        grabImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        
        // do your thang
        ildaFrame.update();
        
        
        // send points to the etherdream
        etherdream.setPoints(ildaFrame);
    
        imageGui.draw();
        
    }else{
        ofBackground(60, 60, 60);
//        cannyImage.draw(0, 0, grabImage.width, grabImage.height);
        
        // draw to the screen
        ildaFrame.draw(0, 0, ofGetWidth(), ofGetHeight());
        ildaRender.draw(0, 0, ofGetWidth(), ofGetHeight());
        ildaRender.getContourFinder().draw(0, 0, 200, 200);
    }
}

void ofApp::mainCommentFBODraw() {
    
    ildaRender.clear();
    ildaRender.begin();
    grabImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    ildaRender.end();
}

void ofApp::serialComDraw() {
    ofSetColor(255, 255, 255);
    commentFont.drawString(tempMessage0, tempMessagePos0.x, tempMessagePos0.y);
    commentFont.drawString(tempMessage1, tempMessagePos1.x, tempMessagePos1.y);
    commentFont.drawString(tempMessage2, tempMessagePos2.x, tempMessagePos2.y);
    commentFont.drawString(tempMessage3, tempMessagePos3.x, tempMessagePos3.y);
    commentFont.drawString(tempMessage4, tempMessagePos4.x, tempMessagePos4.y);
    commentFont.drawString(tempMessage5, tempMessagePos5.x, tempMessagePos5.y);
    commentFont.drawString(tempMessage6, tempMessagePos6.x, tempMessagePos6.y);
    commentFont.drawString(tempMessage7, tempMessagePos7.x, tempMessagePos7.y);
    commentFont.drawString(tempMessage8, tempMessagePos8.x, tempMessagePos8.y);
    commentFont.drawString(tempMessage9, tempMessagePos9.x, tempMessagePos9.y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 's') {
        if (showStr == false) {
            showStr = true;
        } else {
            showStr = false;
        }
    } else if(key == ' ') {
        ildaRender.update(ildaFrame);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit(){
    winManager.saveWindowSettings();
}
