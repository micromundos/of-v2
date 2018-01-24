#pragma once

#include "ofMain.h"
#include "ofxMicromundos/utils.h"
#include "ofxMicromundos/GUI.h"

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
      ofSetFrameRate(30);
      ofBackground(0); 

      projector = ofxMicromundos::projector(true, config);

      ofVec2f proj = ofxMicromundos::projector_position(config);
      ofSetWindowPosition(proj.x, proj.y);
    };

    void update()
    {
      ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
    };

    void draw()
    {
      Backend& backend = app->get_backend();

      //TODO backend: mover calib proj
      //if (backend.calib_enabled() && !projector)
        //projector = ofxMicromundos::projector(true, config);
      //if (!backend.calib_enabled() && projector)
        //projector = ofxMicromundos::projector(false, config);
      //if (!projector)
        //return;

      backend.render_calib(ofGetWidth(),ofGetHeight());
    };

  private:

    shared_ptr<ofApp> app;
    cv::FileStorage config;
    bool projector;
};
