#pragma once

#include "ofMain.h"

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

    void inject(shared_ptr<GUI> gui, cv::FileStorage config, cv::FileStorage server_config, cv::FileStorage plab_config);

    void setup();
    void update();
    void draw();

    void render_monitor(float w, float h);

  private:

    BackendClient backend;
    bool projector;

    Fisica fisica;
    Particles particles;
    FlowField flowfield;
    Bloques bloques;

    shared_ptr<GUI> gui; 
    cv::FileStorage config;
    cv::FileStorage server_config;
    cv::FileStorage plab_config;
};

