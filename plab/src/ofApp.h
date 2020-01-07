#pragma once

#include "ofMain.h"

#include "ofxGPGPU/shaders/gaussian.h"

#include "ofxMicromundos/utils.h"
#include "ofxMicromundos/net/BackendClient.h"
#include "ofxMicromundos/Bloque.h"

#include "plab/GUI.h"
#include "plab/Plab.h"

#ifdef micromundos_USE_SYPHON
  #include "ofxSyphon.h"
  #include "ofxMicromundos/net/syphon/SyphonReceiver.h"
  #include "ofxMicromundos/net/syphon/SyphonSender.h"
#endif

class ofApp : public ofBaseApp
{

  public:

    void inject(shared_ptr<GUI> gui, ofxJSON config, ofxJSON plab_config);

    void setup();
    void update();
    void draw();
    void exit();

    void render_blobs(float w, float h);
    void render_backend_tex(float w, float h, bool blur);
    void render_monitor(float w, float h);

    void update_gaussian(ofShader& shader);

  private:

    BackendClient backend_client;

#ifdef micromundos_USE_SYPHON
    SyphonReceiver syphon_backend;
    SyphonSender syphon_projector;
#endif

    Plab plab;

    gpgpu::Gaussian gaussian; 

    shared_ptr<GUI> gui; 
    ofxJSON config;
    ofxJSON plab_config;
};

