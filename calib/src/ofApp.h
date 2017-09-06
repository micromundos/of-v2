#pragma once

#include "ofMain.h"

#include "ofxMicromundos/data_path.h"
#include "ofxMicromundos/RGBD_Astra.h"
#include "ofxChilitags.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp
{

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


    ofxChilitags chilitags;
    shared_ptr<ofxMicromundos::RGBD> rgbd;

    //TODO
    //TableCalib table;
    //HomographyCalib H_rgb_proy;
    //HomographyCalib H_rgb_depth;
    //HomographyCalib H_depth_proy;

    vector<ofVec2f> proy_pts;
    ofxCv::ContourFinder contourFinder;

};
