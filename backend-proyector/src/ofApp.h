#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxTimeMeasurements.h"

#include "ofxSyphon.h"
#include "ofxMicromundos/net/syphon/SyphonSender.h"
#include "ofxMicromundos/net/syphon/SyphonReceiver.h"

#include "ofxMicromundos/GUI.h"
#include "ofxMicromundos/Backend.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(ofxJSON config, ofxJSON backend_config, shared_ptr<Backend> backend, shared_ptr<GUI> backend_gui);

    void setup();
    void update();
    void draw();

  private:

    shared_ptr<Backend> backend;
    shared_ptr<GUI> backend_gui;

    ofxJSON config;
    ofxJSON backend_config;

    SyphonSender syphon_sender;
    SyphonReceiver syphon_receiver;
};

