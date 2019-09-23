#pragma once

#include "ofMain.h"

#include "ofxGPGPU/shaders/gaussian.h"

#include "ofxSyphon.h"
#include "ofxMicromundos/net/syphon/SyphonReceiver.h"
#include "ofxMicromundos/net/syphon/SyphonSender.h"

#include "ofxMicromundos/utils.h"
#include "ofxMicromundos/net/BackendClient.h"
#include "ofxMicromundos/Bloque.h"

#include "plab/GUI.h"
#include "plab/Fisica.h"
#include "plab/Particles.h"

#include "plab/bloques/Bloques.h"
#include "plab/bloques/Emitter.h"
//#include "plab/bloques/Portal.h"

#include "plab/flowfields/FlowField.h"
#include "plab/flowfields/Container.h"
#include "plab/flowfields/Attractors.h"
//#include "plab/flowfields/Transporter.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(shared_ptr<GUI> gui, ofxJSON config, ofxJSON plab_config);

    void setup();
    void update();
    void draw();
    void exit();

    void render_blobs(float w, float h);
    void render_backend_tex(float w, float h);
    void render_debug(float w, float h);
    void render_monitor(float w, float h);

    void update_gaussian(ofShader& shader);

  private:

    //TODO plab/ofApp MicromundosGame
    BackendClient backend_client;
    SyphonReceiver syphon_receiver;
    SyphonSender syphon_projector;

    Fisica fisica;
    Particles particles;
    FlowField flowfield;
    Bloques bloques;

    gpgpu::Gaussian gaussian; 

    shared_ptr<GUI> gui; 
    ofxJSON config;
    ofxJSON plab_config;
};

