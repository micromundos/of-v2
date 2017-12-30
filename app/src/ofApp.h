#pragma once

#include "ofMain.h"
#include "ofxMicromundos/data_path.h"
#include "RGB.h"
#include "Segmentation.h"
#include "Calib.h"
//#include "Segmentation.h"

class ofApp : public ofBaseApp
{

  public:

    void setup();
    void update();
    void draw();

    RGB rgb;
    Calib calib;
    Segmentation seg;
    ofxChilitags chilitags;
    shared_ptr<CalibGui> gui;

    bool calib_ready;
};

