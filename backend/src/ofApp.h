#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxSyphon.h"

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
    ofxSyphonServer syphon;

    GUI gui; 
    ofxJSON config;
    ofxJSON backend_config;
};

