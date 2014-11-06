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
    ofSetFrameRate(60);
    ofBackground(255);
    ofSetVerticalSync(30);
    
    /* gui */
    imageGui.setup();
    imageGui.add(blurGUI.setup( "blurGUI", 10, 0, 100 ));
    imageGui.add(cannyThresholdGUI.setup("cannyThresholdGUI",ofVec2f(100, 50),ofVec2f(0,0),ofVec2f(255,255)));
    imageGui.add(renderAdaptiveThreashold.setup("renderAdaptiveThreashold",ofVec2f(0, 0),ofVec2f(0,0),ofVec2f(255,255)));
    imageGui.add(renderAdaptiveThreashold2.setup("renderAdaptiveThreashold2",ofVec2f(0, 0),ofVec2f(0,0),ofVec2f(255,255)));
    imageGui.add(smoothAmountGUI.setup( "smoothAmountGUI", 0, 0, 100 ));
    imageGui.add(optimizeToleranceGUI.setup( "optimizeToleranceGUI", 0, 0, 1 ));
    imageGui.add(collapseGUI.setup( "collapseGUI", 0, 0, 100 ));
    imageGui.add(spacingGUI.setup( "spacingGUI.", 0, 0, 100 ));
    
    
    /* UDP Connection */
    udpConnection.Create();
    udpConnection.Bind(10000);
    udpConnection.SetNonBlocking(true);
    
    /* ofxTruetypeFont */
    commentFont.loadFont("Yu Gothic Medium.otf", 70, true, true);
    
    /* コメント管理 */
    commentCounter = 0;
    
    /* laserImageProcessing */
    grabImage.allocate(LASER_WIDTH, LASER_HEIGHT, OF_IMAGE_COLOR);
    blurImage.allocate(LASER_WIDTH, LASER_HEIGHT, OF_IMAGE_COLOR);
    cannyImage.allocate(LASER_WIDTH, LASER_HEIGHT, OF_IMAGE_COLOR);
    ofxDstVid.allocate(LASER_WIDTH, LASER_HEIGHT);
    
    /* effect */
    myFbo.allocate(LASER_WIDTH, LASER_HEIGHT);
    
    // Setup light
	//light.setPosition(1000, 1000, 2000);
    
    /* multi Windows */
    winManager.setup((ofxAppGLFWWindowMulti *)ofGetWindowPtr());
    winManager.loadWindowSettings();
    
    /* LASER */
    etherdream.setup();
    etherdream.setPPS(30000);
    
    /* FRAME */
    ildaFrame.params.draw.lines = true;
    ildaFrame.params.draw.points = true;
    ildaFrame.params.output.transform.doFlipX = false;
    
    /* Render */
    ildaRender.setup(LASER_WIDTH, LASER_HEIGHT);

    
    showStr = true;
    
    grabDispPos.x = ofGetWidth() - LASER_WIDTH - 50;
    grabDispPos.y = 50;
    
    wstring wMessageL = L"トマト";
    comments.push_back(wMessageL);
    
    ofRectangle rect;
    rect = commentFont.getStringBoundingBox(wMessageL, 0, 0);
    float stringWidth = rect.width;
    float stringHeight = rect.height;
    ofPoint point = ofPoint(ofGetWidth() - ofRandom(0, LASER_WIDTH - stringWidth) - LASER_WIDTH, ofRandom(stringHeight, LASER_HEIGHT - stringHeight));
    debugPos.push_back(point);
    
    wMessageL = L"しょうがくせい";
    comments.push_back(wMessageL);
    
    rect;
    rect = commentFont.getStringBoundingBox(wMessageL, 0, 0);
    stringWidth = rect.width;
    stringHeight = rect.height;
    point = ofPoint(ofGetWidth() - ofRandom(0, LASER_WIDTH - stringWidth) - LASER_WIDTH, ofRandom(stringHeight, LASER_HEIGHT - stringHeight));
    debugPos.push_back(point);
    
    wMessageL = L"バナナ";
    comments.push_back(wMessageL);
    
    rect;
    rect = commentFont.getStringBoundingBox(wMessageL, 0, 0);
    stringWidth = rect.width;
    stringHeight = rect.height;
    point = ofPoint(ofGetWidth() - ofRandom(0, LASER_WIDTH - stringWidth) - LASER_WIDTH, ofRandom(stringHeight, LASER_HEIGHT - stringHeight));
    debugPos.push_back(point);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    cannyImage.update();
    blurImage.update();
    
    
    char udpMessage[100000];
    udpConnection.Receive(udpMessage,10000);
    wstring wMessage = stringConvertToW(udpMessage);
    string message = udpMessage;
    if(message !=""){
        cout << message << endl;
        comments.push_back(wMessage);
        
        ofRectangle rect;
        rect = commentFont.getStringBoundingBox(wMessage, 0, 0);
        float stringWidth = rect.width;
        float stringHeight = rect.height;
        ofPoint point = ofPoint(ofGetWidth() - ofRandom(0, LASER_WIDTH - stringWidth) - LASER_WIDTH, ofRandom(stringHeight, LASER_HEIGHT - stringHeight));
        debugPos.push_back(point);
    }
    
    /* 画像処理 */
    /* ぼかし */
    Mat srcBlur, preBlur;
    blur(grabImage, preBlur, blurGUI);
    toOf(preBlur, blurImage);
    
    /* 輪郭抽出 */
    Mat preConvertImage, preCannyImage;
    ofImage srcCannyImage;
    srcCannyImage.setFromPixels(blurImage.getPixels(), LASER_WIDTH, LASER_HEIGHT,  OF_IMAGE_COLOR);
    
    convertColor(srcCannyImage, preConvertImage, CV_RGB2GRAY);
    Canny(preConvertImage, preCannyImage, cannyThresholdGUI->x, cannyThresholdGUI->y, 3);
    toOf(preCannyImage, cannyImage);
    
    /* FBO */
    mainCommentFBODraw();
    
    /* LASER PROCESSING */
    ildaFrame.polyProcessor.params.smoothAmount = smoothAmountGUI;
    ildaFrame.polyProcessor.params.optimizeTolerance = optimizeToleranceGUI;
    ildaFrame.polyProcessor.params.collapse = collapseGUI;
    ildaFrame.polyProcessor.params.spacing = spacingGUI;
    
    ildaFrame.clear();
    ildaRender.update(ildaFrame);
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255, 255, 255);
    if( winManager.getActiveWindowNo() == 0){
        
        if(showStr) serialComDraw();
        
        ofSetColor(255, 255, 255);
        ofRect(grabDispPos.x, grabDispPos.y, LASER_WIDTH, LASER_HEIGHT);
        
        ofSetColor(255);
        grabImage.grabScreen(grabDispPos.x, grabDispPos.y, LASER_WIDTH, LASER_HEIGHT);
        
        // LASER PROCESSING
        ildaFrame.update();
        etherdream.setPoints(ildaFrame);
    
        /* DEBUG GUI */
        imageGui.draw();
        
        ofRect(50, ofGetHeight() - 200 - 50, 200, 200);
        ildaRender.getContourFinder().draw(50, ofGetHeight() - 200 - 50, 200, 200);
        
        ofRect(50 + 200, ofGetHeight() - 200 - 50, 200, 200);
        ildaRender.getGreyImage().draw(50 + 200, ofGetHeight() - 200 - 50, 200, 200);
        
        ofRect(50 + 200 + 200, ofGetHeight() - 200 - 50, 200, 200);
        ildaRender.getColorImage().draw(50 + 200 + 200, ofGetHeight() - 200 - 50, 200, 200);
        
    }else{
        ofBackground(0);
        ildaFrame.draw(0, 0, LASER_WIDTH, LASER_HEIGHT);
        ildaRender.draw(0, 0, LASER_WIDTH, LASER_HEIGHT);
        
    }
}

void ofApp::mainCommentFBODraw() {
    ildaRender.clear();
    ildaRender.begin();
    grabImage.draw(0, 0, LASER_WIDTH, LASER_HEIGHT);
    ildaRender.end();
}

void ofApp::serialComDraw() {
    ofSetColor(255, 255, 255);    
    //for (int i=0; i < comments.size(); i++) {
    
    ofPushMatrix();
    int index = ofGetFrameNum() % comments.size();
    ofTranslate(debugPos[index].x, debugPos[index].y);
    ofRotateZ(ofGetFrameNum() * 2);
    ofRectangle rect = commentFont.getStringBoundingBox(comments[index], 0, 0);
    float stringWidth = rect.width;
    float stringHeight = rect.height;
    commentFont.drawString(comments[index], -rect.width / 2, -rect.height / 2);
    ofPopMatrix();
        //commentFont.drawString(comments[i], ofGetMouseX(), ofGetMouseY());
    //}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 's') {
//        if (showStr == false) {
//            showStr = true;
//        } else {
//            showStr = false;
//        }
    } else if(key == ' ') {
        ildaRender.update(ildaFrame);
    }
    
    switch(key) {
        case 'f': ofToggleFullscreen(); break;
            
            
            // clear the frame
        case 'c': ildaFrame.clear(); break;
            
            // draw rectangle
        case 'r': {
            ofPolyline p = ofPolyline::fromRectangle(ofRectangle(ofRandomuf()/2, ofRandomuf()/2, ofRandomuf()/2, ofRandomuf()/2));
            ildaFrame.addPoly(p);
        }
            break;
            
            // change color
        case 'R': ildaFrame.params.output.color.r = 1 - ildaFrame.params.output.color.r; break;
        case 'G': ildaFrame.params.output.color.g = 1 - ildaFrame.params.output.color.g; break;
        case 'B': ildaFrame.params.output.color.b = 1 - ildaFrame.params.output.color.b; break;
            
            // toggle draw lines (on screen only)
        case 'l': ildaFrame.params.draw.lines ^= true; break;
            
            // toggle loop for last poly
        case 'o': ildaFrame.getLastPoly().setClosed(ildaFrame.getLastPoly().isClosed()); break;
            
            // toggle draw points (on screen only)
        case 'p': ildaFrame.params.draw.points ^= true; break;
            
            // adjust point count
        case '.': ildaFrame.polyProcessor.params.targetPointCount++; break;
        case ',': if(ildaFrame.polyProcessor.params.targetPointCount > 10) ildaFrame.polyProcessor.params.targetPointCount--; break;
            
            // adjust point count quicker
        case '>': ildaFrame.polyProcessor.params.targetPointCount += 10; break;
        case '<': if(ildaFrame.polyProcessor.params.targetPointCount > 20) ildaFrame.polyProcessor.params.targetPointCount -= 10; break;
            
            // flip image
        case 'x': ildaFrame.params.output.transform.doFlipX ^= true; break;
        case 'y': ildaFrame.params.output.transform.doFlipY ^= true; break;
            
            // cap image
        case 'X': ildaFrame.params.output.doCapX ^= true; break;
        case 'Y': ildaFrame.params.output.doCapY ^= true; break;
            
            // move output around
        case OF_KEY_UP: ildaFrame.params.output.transform.offset.y -= 0.05; break;
        case OF_KEY_DOWN: ildaFrame.params.output.transform.offset.y += 0.05; break;
        case OF_KEY_LEFT: ildaFrame.params.output.transform.offset.x -= 0.05; break;
        case OF_KEY_RIGHT: ildaFrame.params.output.transform.offset.x += 0.05; break;
            
            // scale output
        case 'w': ildaFrame.params.output.transform.scale.y += 0.05; break;
        case 's': ildaFrame.params.output.transform.scale.y -= 0.05; break;
        case 'a': ildaFrame.params.output.transform.scale.x -= 0.05; break;
        case 'd': ildaFrame.params.output.transform.scale.x += 0.05; break;
            
        case 'C': ildaFrame.drawCalibration(); break;
            
        case ' ': ildaRender.update(ildaFrame); break;
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
