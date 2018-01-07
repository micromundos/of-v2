#pragma once

#include "ofMain.h"
#include "ofxMicromundos/data_path.h"
#include "GUI.h"
#include "backend/Backend.h"

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

