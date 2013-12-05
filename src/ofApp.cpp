#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
    cropSide = 100;
    angleTotal = 200;
	cam.initGrabber(1280, 720);
    cam.setDeviceID(0);
    
    player.loadMovie("2.mov");
    //player.play();
    
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(200);
	unwarped.allocate(150, 100, OF_IMAGE_COLOR);
}

void testApp::update() {
	cam.update();
    player.update();
    
	if(cam.isFrameNew()) {
    //if(player.isFrameNew()) {
        resize(cam, cvHalfSize, 0.5, 0.5);
        cv::Rect crop(
            cropSide,
            0,
            cvHalfSize.cols - cropSide*2,
            cvHalfSize.rows
        );
        cvCroped = cvHalfSize(crop);

        convertColor(cvCroped, cvGray, CV_RGB2GRAY);
        //equalizeHist(cvGray, cvGray);
        thresholdCv = ofMap(mouseY, 0, ofGetWidth(), 0, 255);
        threshold(cvGray,cvThreshold,thresholdCv);
        
        toOf(cvThreshold,ofHalfSize);
        ofHalfSize.update();
        
		thresholdContour = ofMap(mouseX, 0, ofGetWidth(), 0, 255);
		contourFinder.setThreshold(thresholdContour);
        contourFinder.setInvert(true);
		contourFinder.findContours(cvThreshold);
		
		int n = contourFinder.size();
		quads.clear();
		quads.resize(n);
		for(int i = 0; i < n; i++) {
			quads[i] = contourFinder.getFitQuad(i);
		}
        
        if(quads.size()){
            anglePrev = angle;
            ofVec2f v1(quads[0][0].x,quads[0][0].y);
            ofVec2f v2(quads[0][1].x,quads[0][1].y);
            ofVec2f v3(quads[0][2].x,quads[0][2].y);
            ofVec2f norm(0,1);
            
            ofVec2f diff = v1-v2;
            angle = diff.angle(v2);
            if(abs(angle-anglePrev)<10){
                angleDiff = angle-anglePrev;
                angleTotal+= angleDiff*2;
            }
        }
        
	}
}

void testApp::draw() {
	ofSetColor(255);
	//cam.draw(0, 0);
    ofHalfSize.draw(0, 0);
    
	ofSetLineWidth(2);
	contourFinder.draw();
	
	ofNoFill();
	ofSetColor(magentaPrint);
	for(int i = 0; i < quads.size(); i++) {
		toOf(quads[i]).draw();
	}
    
	ofSetColor(255);
	drawHighlightString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
	drawHighlightString(ofToString((int) thresholdContour) + " thresholdContour", 10, 40);
    drawHighlightString(ofToString((int) thresholdCv) + " thresholdCv", 10, 60);
    drawHighlightString(ofToString((int) angleDiff) + " - " +ofToString((int) angleTotal), 10, 80);
    
    if(quads.size()){
        drawHighlightString(ofToString(quads[0][0].x) + " " + ofToString(quads[0][0].y), 10, 100);
        
        ofPushMatrix();
        ofSetColor(255, 0, 0);
        ofFill();
        ofTranslate(angleTotal, 400);
        //ofRotate(angleTotal);
        //ofLine(0, 0, 50, 50);
        ofRect(0, 0, 40, 40);
        ofPopMatrix();
    }
    

	
}

void testApp::mousePressed(int x, int y, int button) {
	targetColor = cam.getPixelsRef().getColor(x, y);
	//contourFinder.setTargetColor(targetColor, TRACK_COLOR_HSV);
}

void testApp::keyPressed(int key) {
}