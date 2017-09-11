#include "ofApp.h"
#include "ofxGPGPU.h"

void ofApp::setup()
{
  ofxMicromundos::setDataPathRoot();
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  gpgpu::Process::watch("all");
  calib.init(gui);
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
  calib.update();
};

void ofApp::draw()
{
  float w = ofGetWidth();
  float h = ofGetHeight();
  calib.render(0, 0, w, h);
};

