#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxTimeMeasurements.h"

#include "ofxMicromundos/GUI.h"
#include "ofxMicromundos/Backend.h"

#ifdef micromundos_USE_SYPHON
  #include "ofxSyphon.h"
  #include "ofxMicromundos/net/syphon/SyphonSender.h"
#endif

class ofApp : public ofBaseApp
{

  public:

    void inject(ofxJSON config, ofxJSON backend_config);

    void setup();
    void update();
    void draw();

  private:

    Backend backend;
    GUI gui; 
    ofxJSON config;
    ofxJSON backend_config;

#ifdef micromundos_USE_SYPHON
    SyphonSender syphon_sender;
#endif
};

