#include "ofApp.h"

void ofApp::inject(shared_ptr<Backend> backend, shared_ptr<GUI> gui)
{
  this->backend = backend;
  this->gui = gui;
};

void ofApp::setup()
{
  ofxMicromundos::setDataPathRoot();
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetWindowPosition(ofGetScreenWidth()-ofGetWidth(), 0);
  ofBackground(0);
  backend->init(gui);
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
  backend->update();
};

void ofApp::draw()
{  
  backend->render();
};

