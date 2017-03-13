#pragma once

#include "ofMain.h"
//#include "ofxGui.h"

#include "ofxOpenCv.h"

// aux functions

ofRectangle getBoundaryRct(const std::vector<ofRectangle> &rcts);


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
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void updateFrames();

	void getIntersections();

	void generateMerged();

	//ofVideoPlayer vids[4];
	ofVideoPlayer vid;

	ofImage imgs[4];

	std::vector<ofRectangle> frames;
	std::vector<ofRectangle> intersections;

	int offset=20;
	int vidW;
	int vidH;
	int vidWOrig;
	int vidHOrig;

	ofImage imgMerged;
	ofRectangle vidSpace;
	ofPoint pntVid[4];

	ofFbo fbo;

	// dragging
	ofPoint dragStart;
	ofPoint dragRel;
	ofPoint dragPnt;
	bool drawDrag;
	int whichToDrag;

//	//parameters
//	ofxPanel gui;
//	ofxIntSlider vidXSlider[4];
//	ofxIntSlider vidYSlider[4];

};
