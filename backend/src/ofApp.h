#pragma once

#include "ofMain.h"
#include "GUI.h"
#include "ofxMicromundos/Backend.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(cv::FileStorage config, cv::FileStorage backend_config);

    void setup();
    void update();
    void draw();

  private:

    Backend backend;

    GUI gui; 
    cv::FileStorage config;
    cv::FileStorage backend_config;
};

