#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxTimeMeasurements.h"

#include "ofxSyphon.h"
#include "ofxMicromundos/net/syphon/SyphonSender.h"

//TODO backend app move GUI to ofxMicromundos
#include "GUI.h"
#include "ofxMicromundos/Backend.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(ofxJSON config, ofxJSON backend_config);

    void setup();
    void update();
    void draw();

  private:

    Backend backend;
    SyphonSender syphon_sender;

    GUI gui; 
    ofxJSON config;
    ofxJSON backend_config;
};

