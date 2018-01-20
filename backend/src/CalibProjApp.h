#pragma once

#include "ofMain.h"
#include "GUI.h"

class CalibProjApp: public ofBaseApp 
{
  public: 

    void inject(shared_ptr<ofApp> app, cv::FileStorage config) 
    {
      this->app = app;
      this->config = config;
    };

    void setup() 
    {
      ofSetVerticalSync(true);
      ofBackground(0); 

      float xoff = config["projector_x_offset_from_desktop_width"];
      ofSetWindowPosition(ofGetScreenWidth() + xoff, config["projector_y"]);
    };

    void update()
    {
      ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
    };

    void draw()
    {
      app->get_backend().render_calib(ofGetWidth(),ofGetHeight());
    };

  private:

    shared_ptr<ofApp> app;
    cv::FileStorage config;
};
