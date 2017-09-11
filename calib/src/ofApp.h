#pragma once

#include "ofMain.h"
#include "ofxMicromundos/data_path.h"
#include "Calib.h"

class ofApp : public ofBaseApp
{

  public:

    void setup();
    void update();
    void draw();

    Calib calib;
    shared_ptr<CalibGui> gui;
};

