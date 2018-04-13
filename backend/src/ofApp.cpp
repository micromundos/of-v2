#include "ofApp.h"

void ofApp::inject(ofxJSON config, ofxJSON backend_config)
{
  this->config = config;
  this->backend_config = backend_config;
};

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetFrameRate(30);
  ofBackground(40);

  gui.init((float)ofGetWidth()/2); 

  backend.init(
      config["projector"]["width"].asFloat(), 
      config["projector"]["height"].asFloat(),  
      backend_config["cam"]["width"].asFloat(), 
      backend_config["cam"]["height"].asFloat(), 
      backend_config["cam"]["device_id"].asInt(),
      backend_config["calib"]["H_cam_proj"].asString(),
      backend_config["calib"]["cam_calib"].asString(),
      backend_config["network"]["resize_pixels"].asFloat(),
      config["calib"]["tag_id"].asInt(),
      config["calib"]["proj_pts"],
      config["juegos"],
      config["backend"]["port_bin"].asInt(),
      config["backend"]["port_msg"].asInt());

  syphon.setName(config["backend"]["syphon"].asString());
};

void ofApp::update()
{
  if (!backend.update())
    return;

  backend.send(gui.send_message, gui.send_binary, gui.send_syphon);

  if (gui.send_syphon)
    syphon.publishTexture(&backend.projected_texture());
};

void ofApp::draw()
{  
  float w = ofGetWidth();
  float h = ofGetHeight();

  ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),2), 10, 14, ofColor::yellow, ofColor::black);

  float _w = w/2;  
  float _h = h/3;  
  float lh = 24;

  gui.render(0, lh); 

  backend.print_connection(0, _h+lh);

  backend.print_metadata(0, _h*2);
  backend.print_bloques(0, _h*2.3);

  if (gui.backend_monitor)
    backend.render_monitor(_w, 0, _w, h);
};

