#pragma once

#include "ofMain.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/Bloque.h"
#include "GUI.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(shared_ptr<GUI> gui, cv::FileStorage config);

    void setup();
    void update();
    void draw();

    Backend& get_backend() { return backend; };

  private:

    Backend backend;

    shared_ptr<GUI> gui; 
    cv::FileStorage config;
};

