#include "ofApp.h"

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetFrameRate(60);
  ofBackground(0);

  cv::FileStorage config = ofxMicromundos::load_config("config.yml");
  ofxMicromundos::projector(config);

  int port_bin = 0;

  backend_client.init(
      config["backend"]["ip"],
      port_bin,
      config["backend"]["port_msg"],
      config["projector"]["width"], 
      config["projector"]["height"],
      config["calib"]["proj_pts"]);

  syphon_receiver.init(config["projector"]["syphon"]);
}

void ofApp::draw()
{
  float w = ofGetWidth();
  float h = ofGetHeight();

  backend_client.update();

  if (backend_client.render_calib(w, h))
    return;

  syphon_receiver.render_texture(0, 0, w, h);
}

