#include "ofApp.h"

void ofApp::inject(shared_ptr<GUI> gui, cv::FileStorage config)
{
  this->gui = gui;
  this->config = config;
};

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetWindowPosition(ofGetScreenWidth()
      -ofGetWidth()
      , 0);
  ofBackground(0);

  backend.init(ofGetWidth(), ofGetHeight(), config["rgb_width"], config["rgb_height"], config["rgb_device_id"]);
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

  backend.update(ofGetWidth(), ofGetHeight());
};

void ofApp::draw()
{  
  float w = ofGetWidth();
  float h = ofGetHeight();

  if (gui->backend_debug_pixels)
    backend.render_projected_pixels(w, h);

  if (gui->backend_debug_tags)
    backend.render_projected_tags();

  backend.render_calib(w, h);
};

