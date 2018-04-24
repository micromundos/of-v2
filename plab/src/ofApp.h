#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"
#include "ofxMicromundos/SyphonReceiver.h"

#include "ofxMicromundos/utils.h"
#include "ofxMicromundos/BackendClient.h"
#include "ofxMicromundos/Bloque.h"

#include "plab/GUI.h"
#include "plab/Fisica.h"
#include "plab/Particles.h"

#include "plab/bloques/Bloques.h"
#include "plab/bloques/Emitter.h"

#include "plab/flowfields/FlowField.h"
#include "plab/flowfields/Container.h"
#include "plab/flowfields/Attractors.h"
#include "plab/flowfields/Transporter.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(shared_ptr<GUI> gui, ofxJSON config, ofxJSON plab_config);

    void setup();
    void update();
    void draw();

    void render_debug(float w, float h);
    void render_monitor(float w, float h);

  private:

    BackendClient backend_client;
    SyphonReceiver syphon_receiver;
    ofxSyphonServer projector_syphon;

    Fisica fisica;
    Particles particles;
    FlowField flowfield;
    Bloques bloques;

    shared_ptr<GUI> gui; 
    ofxJSON config;
    ofxJSON plab_config;
};

