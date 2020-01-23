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
    void keyReleased(int key);

  private:

    Backend backend;
    GUI gui; 
    ofxJSON config;
    ofxJSON backend_config;

    bool render_cam;
    void toggle_cam();
    void cam_window();
    void backend_window();

#ifdef micromundos_USE_SYPHON
    SyphonSender syphon_sender;
#endif
};

