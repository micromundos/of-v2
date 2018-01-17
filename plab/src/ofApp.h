#pragma once

#include "ofMain.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/Bloque.h"
#include "GUI.h"

#include "plab/Fisica.h"
#include "plab/Particles.h"
#include "plab/flowfields/FlowField.h"
#include "plab/flowfields/FlowFieldContainer.h"

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
    //TODO Bloques bloques;

    shared_ptr<GUI> gui; 
    cv::FileStorage config;
};

