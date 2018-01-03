#pragma once

#include "ofMain.h"
#include "ofApp.h"
#include "GUI.h"

class BackendMonitor: public ofBaseApp 
{
  public:

    shared_ptr<ofApp> app;
    shared_ptr<GUI> gui;

    void setup()
    {
    };

    void update()
    {
      ofSetWindowTitle(ofToString(app->fps,2));
    };

    void draw()
    {
      if (gui->backend_monitor)
        render_backend();
    };

  private:

    void render_backend()
    {
      float w = ofGetWidth();
      float h = ofGetHeight();

      float hw = w/2;
      float hh = h/2;

      //top left
      app->rgb.render(0, 0, hw, hh);
      app->chilitags.render(0, 0, hw, hh);

      //top right
      app->seg.render(hw, 0, hw, hh);
    };
};


