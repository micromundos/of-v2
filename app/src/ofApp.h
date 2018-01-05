#pragma once

#include "ofMain.h"
#include "ofxMicromundos/data_path.h"
#include "GUI.h"
#include "RGB.h"
#include "Calib.h"
#include "Segmentation.h"

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
    shared_ptr<GUI> gui;

    float fps;
    float calib_enabled;

    ofPixels proj_pix;
    ofTexture proj_tex;
    vector<ChiliTag> proj_tags;

    void render_proj_pix();
    void render_proj_tags();
};

