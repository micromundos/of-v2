#pragma once

#include "ofMain.h"
#include "ofxMicromundos/utils.h"
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

      ofVec2f proj = ofxMicromundos::projector_position(config);
      ofSetWindowPosition(proj.x, proj.y);
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
