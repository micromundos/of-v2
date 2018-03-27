#include "ofApp.h"

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetFrameRate(60);
  ofBackground(0);

  cv::FileStorage config = ofxMicromundos::load_config("config.yml");
  ofxMicromundos::projector(config);

  syphon_receiver.init(config["projector"]["syphon"]);
}

void ofApp::draw()
{
  syphon_receiver.render_texture(0, 0, ofGetWidth(), ofGetHeight());
}

