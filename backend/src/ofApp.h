#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxTimeMeasurements.h"

#include "ofxMicromundos/GUI.h"
#include "ofxMicromundos/Backend.h"

#ifdef TARGET_OSX
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

#ifdef TARGET_OSX
    SyphonSender syphon_sender;
#endif
};

