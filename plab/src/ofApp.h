#pragma once

#include "ofMain.h"
#include "ofxMicromundos/data_path.h"
#include "ofxMicromundos/Backend.h"
#include "GUI.h"

#include "plab/Fisica.h"
#include "plab/Particles.h"

class ofApp : public ofBaseApp
{

  public:

    void inject(shared_ptr<Backend> backend, shared_ptr<GUI> gui);

    void setup();
    void update();
    void draw();

  private:

    Fisica fisica;
    Particles particles;

    shared_ptr<Backend> backend;
    shared_ptr<GUI> gui;
};

