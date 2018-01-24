#pragma once

#include "ofMain.h"
#include "plab/GUI.h"

class MonitorApp : public ofBaseApp 
{
  public: 

    void inject(shared_ptr<ofApp> app, shared_ptr<GUI> gui) 
    {
      this->app = app;
      this->gui = gui;
    };

    void setup() 
    {
      ofSetFrameRate(30);
      ofBackground(40);
      gui->init((float)ofGetWidth()/2);
    };

    void update()
    {
      ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
    };

    void draw()
    {
      app->render_monitor(ofGetWidth(), ofGetHeight());
    };

  private:

    shared_ptr<ofApp> app;
    shared_ptr<GUI> gui;
};

