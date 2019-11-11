#include "ofApp.h"

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetFrameRate(60);
  ofBackground(0);

  ofxJSON config = ofxMicromundos::load_config("config.json");
  ofxMicromundos::projector(config);

  backend_client.init(
      config["backend"]["ip"].asString(),
      config["backend"]["port_bin"].asInt(),
      config["backend"]["port_msg"].asInt(),
      config["backend"]["port_blobs"].asInt(),
      config["projector"]["width"].asFloat(), 
      config["projector"]["height"].asFloat());
}

void ofApp::draw()
{
  float w = ofGetWidth();
  float h = ofGetHeight();

  backend_client.update();

  if (backend_client.calib_enabled())
    backend_client.render_calib();

  backend_client.render_texture(0, 0, w, h);
}

