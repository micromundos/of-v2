#pragma once

#include "ofMain.h"
#include "ofxMicromundos/Backend.h"
#include "GUI.h"

class BackendMonitorApp: public ofBaseApp 
{
  public: 

    void inject(shared_ptr<ofApp> app, shared_ptr<GUI> gui) 
    {
      this->app = app;
      this->gui = gui;
    };

    void setup() 
    {};

    void update()
    {
      ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
    };

    void draw()
    {
      if (!gui->backend_monitor)
        return;

      float w = ofGetWidth();
      float h = ofGetHeight();

      app->get_backend().render_monitor(0, 0, w/2, h/2);
    };

  private:

    shared_ptr<ofApp> app;
    shared_ptr<GUI> gui;
};


