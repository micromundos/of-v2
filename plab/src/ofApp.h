#pragma once

#include "ofMain.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/Bloque.h"
#include "GUI.h"

#include "plab/Fisica.h"
#include "plab/Particles.h"
#include "plab/flowfields/FlowField.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(shared_ptr<GUI> gui, cv::FileStorage config);

    void setup();
    void update();
    void draw();

    Backend& get_backend() { return backend; };
    FlowField& get_flowfield() { return flowfield; };

  private:

    Backend backend;
    Fisica fisica;
    Particles particles;
    FlowField flowfield;

    void parse_input(ofPixels& src, ofFloatPixels& dst_pix, ofTexture& dst_tex);
    ofTexture input_tex;
    ofFloatPixels input_pix;

    shared_ptr<GUI> gui; 
    cv::FileStorage config;
};

