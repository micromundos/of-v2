#include "ofApp.h"

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetFrameRate(60);
  ofBackground(0);

  ofxJSON config = ofxMicromundos::load_config("config.json");
  ofxMicromundos::projector(config);

  int port_blobs_disabled = 0;

  backend_client.init(
      config["backend"]["ip"].asString(),
      config["backend"]["port_bin"].asInt(),
      config["backend"]["port_msg"].asInt(),
      port_blobs_disabled,
      config["projector"]["width"].asFloat(), 
      config["projector"]["height"].asFloat());

  syphon_projector.init(config["projector"]["syphon"].asString());
  syphon_backend.init(config["backend"]["syphon"].asString());
}

void ofApp::draw()
{
  float w = ofGetWidth();
  float h = ofGetHeight();

  backend_client.update();

  if (backend_client.calib_enabled())
  {
    if (backend_client.syphon_enabled())
      syphon_backend.render_texture(0, 0, w, h);
    else
      backend_client.render_texture(0, 0, w, h);

    backend_client.render_calib();
    return;
  }

  syphon_projector.render_texture(0, 0, w, h);
}

