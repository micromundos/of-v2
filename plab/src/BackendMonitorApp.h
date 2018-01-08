#pragma once

#include "ofMain.h"
#include "ofxMicromundos/Backend.h"
#include "GUI.h"

class BackendMonitorApp: public ofBaseApp 
{
  public: 

    void inject(shared_ptr<Backend> backend, shared_ptr<GUI> gui) 
    {
      this->backend = backend;
      this->gui = gui;
    };

    void setup() {};

    void update()
    {
      ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
    };

    void draw()
    {
      if (!gui->backend_monitor)
        return;
      backend->render_monitor(ofGetWidth(), ofGetHeight()); 
    };

  private:

    shared_ptr<Backend> backend;
    shared_ptr<GUI> gui;
};


