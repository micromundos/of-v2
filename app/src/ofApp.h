#pragma once

#include "ofMain.h"
#include "ofxMicromundos/Backend.h"
#include "GUI.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(shared_ptr<Backend> backend, shared_ptr<GUI> gui);

    void setup();
    void update();
    void draw();

  private:

    shared_ptr<Backend> backend;
    shared_ptr<GUI> gui;
};

