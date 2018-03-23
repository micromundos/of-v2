#pragma once

#include "ofMain.h"
#include "BackendSyphon.h"
#include "ofxSyphon.h"

#include "ofxMicromundos/utils.h"
#include "ofxMicromundos/BackendClient.h"
#include "ofxMicromundos/Bloque.h"

#include "plab/GUI.h"
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

    void render_monitor(float w, float h);

  private:

    BackendClient backend_client;
    BackendSyphon backend_syphon;
    ofxSyphonServer plab_syphon;

    Fisica fisica;
    Particles particles;
    FlowField flowfield;
    Bloques bloques;

    bool projector;

    shared_ptr<GUI> gui; 
    cv::FileStorage config;
    cv::FileStorage plab_config;
};

