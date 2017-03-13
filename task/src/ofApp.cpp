#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	string srcvid="movies/scene.avi";
	//string srcvid="movies/ISTrailer.mp4";

	if(vid.load(srcvid)){

		ofSetVerticalSync(true);

		fbo.allocate(1280, 700, GL_RGBA);

		vid.setLoopState(OF_LOOP_NORMAL);
		vid.play();

		vidWOrig=vid.getWidth();
		vidHOrig=vid.getHeight();

		vidW=vidWOrig*0.6;
		vidH=vidHOrig*0.6;

		for(int i=0; i<4; i++){
			imgs[i].allocate(vidWOrig, vidHOrig, OF_IMAGE_GRAYSCALE);
			//imgs[i].allocate(vidW, vidH);
		}
	//	imgs[0].crop(0, 0, vidW*0.6, vidH*0.6);
	//	imgs[1].crop(vidW*0.4, 0, vidW*0.6, vidH*0.6);
	//	imgs[2].crop(0, vidH*0.4, vidW*0.6, vidH*0.6);
	//	imgs[3].crop(vidW*0.4,vidH*0.4, vidW*0.6, vidH*0.6);

	//	vidW*=0.6;
	//	vidH*=0.6;

		// videos map:
		//	1	2
		//	3	4
		pntVid[0]=ofPoint(offset, offset);
		pntVid[1]=pntVid[0]+ofPoint(vidW, 0);
		pntVid[2]=pntVid[0]+ofPoint(0, vidH);
		pntVid[3]=pntVid[0]+ofPoint(vidW, vidH);


		for(int i=0; i<4; i++){
			frames.push_back(ofRectangle(pntVid[i], pntVid[i]+ofPoint(vidW, vidH)));
		}

		vidSpace=getBoundaryRct(frames);

		drawDrag=false;

		updateFrames();

	//	gui.setup();
	//	for(int i=0; i<4; i++){
	//		// from where to get screen size?
	//		gui.add(vidXSlider[i].setup("vid" + ofToString(i+1) + " x", pntVid[i].x, 0, 1280));
	//		gui.add(vidYSlider[i].setup("vid" + ofToString(i+1) + " y", pntVid[i].y, 0, 700));
	//	}
	}
	else{
		ofExit();
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	vid.update();
	for(int i=0; i<4; i++){
		imgs[i].setFromPixels(vid.getPixels());
		imgs[i].setImageType(OF_IMAGE_GRAYSCALE);
		//imgs[i].setFromPixels(vid.getPixels().getData(), vidW, vidH, OF_IMAGE_GRAYSCALE);
	}

	imgs[0].crop(0, 0, vidW, vidH);
	imgs[1].crop(vidWOrig-vidW, 0, vidW, vidH);
	imgs[2].crop(0, vidHOrig-vidH, vidW, vidH);
	imgs[3].crop(vidWOrig-vidW, vidHOrig-vidH, vidW, vidH);

	generateMerged();
}

//--------------------------------------------------------------
void ofApp::draw(){

	fbo.begin();
	ofClear(255);

	ofSetHexColor(0xFFFFFF);

	for(int i=0; i<4; i++){
//		ofPushStyle();
//		ofEnableAlphaBlending();
//		ofSetColor(255, 128);
		//vids[i].draw(pntVid[i]);
		imgs[i].draw(pntVid[i]);
//		ofDisableAlphaBlending();
		ofSetHexColor(0x00FF00);
		ofDrawBitmapString("Cam " + ofToString(i+1), pntVid[i]);
		ofSetHexColor(0xFFFFFF);
		//ofPopStyle();
	}

	//gui.draw();

	ofNoFill();
	for(int i=0; i<intersections.size(); i++){

		ofSetHexColor(0xFF0000);
		ofDrawRectangle(intersections[i].x, intersections[i].y, intersections[i].width, intersections[i].height);
		ofSetHexColor(0xFFFFFF);
	}


	ofSetHexColor(0xFF8800);
	if(drawDrag){
		ofDrawRectangle(dragPnt, vidW, vidH);
	}
	ofDrawRectangle(vidSpace);
	ofSetHexColor(0xFFFFFF);

	//generateMerged();
	imgMerged.draw(vidSpace.x+vidSpace.width+offset, offset);

	fbo.end();
	fbo.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// Z: undo last movement
	switch(key){
		case 'z':
			if(whichToDrag<0 || whichToDrag>3)
				break;
			pntVid[whichToDrag]-=dragRel;
			whichToDrag=-1;
			updateFrames();
		break;
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
	int n=0;
	for(int i=0; i<4; i++){
		if(frames[i].inside(dragStart)){
			n++;
			if(n>1){
				drawDrag=false;
				return;
			}
			dragPnt=ofPoint(x,y)-dragStart+frames[i].getPosition();
		}
	}
	if(n){
		drawDrag=true;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
		dragStart=ofPoint(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	int n=0;
	for(int i=0; i<4; i++){
		if(frames[i].inside(dragStart)){
			n++;
			if(n>1) return;
			whichToDrag=i;
		}
	}

	dragRel=ofPoint(x,y)-dragStart;

	pntVid[whichToDrag]+=dragRel;

	if(pntVid[whichToDrag].x < vidSpace.x)
		pntVid[whichToDrag].x = vidSpace.x;
	if(pntVid[whichToDrag].y < vidSpace.y)
		pntVid[whichToDrag].y = vidSpace.y;
	if(pntVid[whichToDrag].x+frames[whichToDrag].width > vidSpace.x+vidSpace.width)
		pntVid[whichToDrag].x = vidSpace.x+vidSpace.width-frames[whichToDrag].width;
	if(pntVid[whichToDrag].y+frames[whichToDrag].height > vidSpace.y+vidSpace.height)
		pntVid[whichToDrag].y = vidSpace.y+vidSpace.height-frames[whichToDrag].height;

	updateFrames();
	drawDrag=false;
	getIntersections();}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
void ofApp::updateFrames(){
	for(int i=0; i<4; i++){
		frames[i]=ofRectangle(pntVid[i], ofPoint(pntVid[i].x+vidW, pntVid[i].y+vidH));
	}

}


//--------------------------------------------------------------
void ofApp::getIntersections(){
	ofRectangle is;
	intersections.clear();
	for(int i=0; i<frames.size(); i++){
		for(int j=i+1; j<frames.size() && j!=i; j++){
			is=frames[i].getIntersection(frames[j]);
			if(!is.isEmpty()){
				intersections.push_back(is);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::generateMerged(){
	//ofRectangle area=getBoundaryRct(intersections);getIntersections();
	ofRectangle area=getBoundaryRct(frames);
	ofSetHexColor(0x0000FF);
	ofDrawRectangle(area);
	ofSetHexColor(0xFFFFFF);

	imgMerged.allocate(area.width, area.height, OF_IMAGE_GRAYSCALE);
	//imgMerged.allocate(area.width, area.height);
	for(int x=0; x<area.width; x++){
		for(int y=0; y<area.height; y++){
			ofPoint act(area.x+x, area.y+y);
			int n=0;
			int r=0, g=0, b=0;
			for(int i=0; i<frames.size(); i++){
				if(frames[i].inside(act)){
					//ofPixels &pxls=vids[i].getPixels();
					ofPixels &pxls=imgs[i].getPixels();

					ofPoint vidPt=act-frames[i].position;

					//int px=3*(vidW*vidPt.y+vidPt.x);
					int px=vidW*vidPt.y+vidPt.x;
					r+=pxls[px];
			//		g+=pxls[px+1];
			//		b+=pxls[px+2];
					n++;
				}
			}

			if(n>1){
				r/=n;// g/=n; b/=n;
			}
			//int pxl=3*(area.width*y+x);
			int pxl=area.width*y+x;
			imgMerged.getPixels()[pxl]=r;
//			imgMerged.getPixels()[pxl+1]=g;
//			imgMerged.getPixels()[pxl+2]=b;
		}
	}

}


// AUX

ofRectangle getBoundaryRct(const std::vector<ofRectangle> &rcts){
	if(rcts.size()==0) return ofRectangle(0,0,0,0);
	ofPoint min(1280, 700), max(0, 0);
	for(int i=0; i<rcts.size(); i++){
		if(rcts[i].x<min.x) min.x=rcts[i].x;
		if(rcts[i].y<min.y) min.y=rcts[i].y;
		if(rcts[i].x+rcts[i].width>max.x) max.x=rcts[i].x+rcts[i].width;
		if(rcts[i].y+rcts[i].height>max.y) max.y=rcts[i].y+rcts[i].height;
	}
	return ofRectangle(min, max);
}
