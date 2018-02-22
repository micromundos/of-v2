#include "ofApp.h"

void ofApp::inject(shared_ptr<GUI> gui, cv::FileStorage config, cv::FileStorage server_config)
{
  this->gui = gui;
  this->config = config;
  this->server_config = server_config;
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
      server_config["network"]["port_bin"],
      server_config["network"]["port_msg"]);
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

  if (!backend.update())
    return;

  if (gui->send)
    backend.send(server_config["network"]["resize_pixels"]);
};

void ofApp::draw()
{  
  float w = ofGetWidth();
  float h = ofGetHeight();

  float h_ = h/3;

  if (gui->backend_monitor)
    backend.render_monitor(0, 0, w, h_);

  backend.render_server_info(0, h_+20);

  gui->render(w/2, h_);
};

