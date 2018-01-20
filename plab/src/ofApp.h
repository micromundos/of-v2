#pragma once

#include "ofMain.h"

#include "ofxMicromundos/utils.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/Bloque.h"

#include "GUI.h"

#include "plab/Fisica.h"
#include "plab/Particles.h"

#include "plab/flowfields/FlowField.h"
#include "plab/flowfields/FlowFieldContainer.h"

#include "plab/bloques/Bloques.h"
#include "plab/bloques/Emitter.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(shared_ptr<GUI> gui, cv::FileStorage config, cv::FileStorage plab_config);

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
    Bloques bloques;

    shared_ptr<GUI> gui; 
    cv::FileStorage config;
    cv::FileStorage plab_config;
};

