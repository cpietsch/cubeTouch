#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	
	ofVideoGrabber cam;	
	ofxCv::ContourFinder contourFinder;
	vector< vector<cv::Point> > quads;
	float thresholdContour, thresholdCv;
	ofColor targetColor;
	ofImage unwarped;
    
    ofVideoPlayer player;
    cv::Mat cvCroped,cvHalfSize,cvGray,cvThreshold;
    ofImage ofHalfSize;
    int cropSide;
    float angle,anglePrev,angleDiff,angleTotal;
};
