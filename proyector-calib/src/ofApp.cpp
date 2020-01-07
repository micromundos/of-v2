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

void ofApp::update()
{
  backend_client.update();
};

void ofApp::draw()
{
  //use as test app (not only for calibration)
  //if (!backend_client.calib_enabled())
    //return;

  float w = ofGetWidth();
  float h = ofGetHeight();

  backend_client.render_texture(0, 0, w, h);
  backend_client.render_calib();
  render_blobs(w, h);
}

void ofApp::render_blobs(float w, float h)
{
  ofPushStyle();
  ofSetColor(ofColor::magenta);
  ofSetLineWidth(4);
  backend_client.render_blobs(0, 0, w, h);
  ofPopStyle();
};

