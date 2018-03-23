#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"

#include "GUI.h"
#include "ofxMicromundos/Backend.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(cv::FileStorage config, cv::FileStorage backend_config, cv::FileStorage cartuchos_config);

    void setup();
    void update();
    void draw();

  private:

    Backend backend;
    ofxSyphonServer syphon;

    GUI gui; 
    cv::FileStorage config;
    cv::FileStorage backend_config;
    cv::FileStorage cartuchos_config;
};

