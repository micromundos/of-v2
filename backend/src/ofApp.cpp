#include "ofApp.h"

void ofApp::inject(shared_ptr<GUI> gui, cv::FileStorage config, cv::FileStorage backend_config)
{
  this->gui = gui;
  this->config = config;
  this->backend_config = backend_config;
};

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetFrameRate(30);
  ofBackground(40);

  gui->init((float)ofGetWidth()/2); 

  backend.init(
      config["projector"]["width"], 
      config["projector"]["height"],  
      config["cam"]["width"], 
      config["cam"]["height"], 
      config["cam"]["device_id"],
      config["calib"]["H_cam_proj"],
      config["calib"]["cam_calib"],
      config["calib"]["tag_id"],
      config["calib"]["proj_pts"],
      backend_config["network"]["resize_pixels"],
      backend_config["network"]["port_bin"],
      backend_config["network"]["port_msg"]);
};

void ofApp::update()
{
  if (!backend.update())
    return;

  if (gui->send)
    backend.send();
};

void ofApp::draw()
{  
  float w = ofGetWidth();
  float h = ofGetHeight();

  if (gui->backend_monitor)
    backend.render_monitor(0, 20, w, h/3-20); 

  backend.print_server_info(0, h/3+20);
  gui->render(w/2, h/3);

  if (gui->print_bloques)
    backend.print_bloques(0, 2*h/3);

  ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),2), 10, 14, ofColor::yellow, ofColor::black);
};

