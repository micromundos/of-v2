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
  ofBackground(40);

  backend.init(
      config["projector"]["width"], 
      config["projector"]["height"],  
      config["cam"]["width"], 
      config["cam"]["height"], 
      config["cam"]["device_id"],
      config["calib"]["file"],
      config["calib"]["tag_id"],
      config["calib"]["tags_size"]);
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

  if (!backend.update())
    return;

  ofPixels& proj_pix = backend.projected_pixels();
  map<int, Bloque>& proj_bloques = backend.projected_bloques();
  //TODO pix + bloques -> tcp
};

void ofApp::draw()
{  
  float w = ofGetWidth();
  float h = ofGetHeight();

  if (gui->backend_monitor)
    backend.render_monitor(0, 0, w, h*0.333);
};
