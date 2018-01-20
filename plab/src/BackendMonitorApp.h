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
    {
      ofBackground(40);
    };

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

      app->get_backend().render_monitor(0, 0, w, h*0.3);
      app->get_flowfield().render_monitor(0, h*0.3, w, h*0.7);
    };

  private:

    shared_ptr<ofApp> app;
    shared_ptr<GUI> gui;
};


