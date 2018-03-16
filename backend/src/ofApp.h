#pragma once

#include "ofMain.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/GUI.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(shared_ptr<GUI> gui, cv::FileStorage config, cv::FileStorage backend_config);

    void setup();
    void update();
    void draw();

  private:

    Backend backend;

    shared_ptr<GUI> gui; 
    cv::FileStorage config;
    cv::FileStorage backend_config;
};

