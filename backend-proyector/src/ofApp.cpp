#include "ofApp.h"

//#define TIME_MEASUREMENTS_DISABLED

void ofApp::inject(ofxJSON config, ofxJSON backend_config, shared_ptr<Backend> backend, shared_ptr<GUI> backend_gui)
{
  this->config = config;
  this->backend_config = backend_config;
  this->backend = backend;
  this->backend_gui = backend_gui;
};

void ofApp::setup()
{
  TIME_SAMPLE_SET_FRAMERATE(30.0f);
  TIME_SAMPLE_SET_AVERAGE_RATE(0.05);

  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetFrameRate(30);
  ofBackground(0);

  ofxMicromundos::projector(config);

  backend->init(
    config["projector"]["width"].asFloat(), 
    config["projector"]["height"].asFloat(),  
    backend_config["cam"]["width"].asFloat(), 
    backend_config["cam"]["height"].asFloat(), 
    backend_config["cam"]["device_id"].asInt(),
    backend_config["calib"]["H_cam_proj"].asString(),
    backend_config["calib"]["cam_calib"].asString(),
    backend_config["chilitags"]["fps"].asFloat(),
    backend_config["resize"]["segmentation"].asFloat(),
    backend_config["resize"]["send"].asFloat(),
    config["calib"]["tag_id"].asInt(),
    config["juegos"],
    config["backend"]["port_bin"].asInt(),
    config["backend"]["port_msg"].asInt(),
    config["backend"]["port_blobs"].asInt());

  syphon_sender
    .init(config["backend"]["syphon"].asString())
    .start();

  syphon_receiver.init(config["projector"]["syphon"].asString());
};

void ofApp::update()
{
  if (!backend->update())
    return;

  backend->send(
    backend_gui->send_message, 
    backend_gui->send_binary, 
    backend_gui->send_syphon, 
    backend_gui->send_blobs);

  if (backend_gui->send_syphon)
    syphon_sender.publishTexture(&(backend->texture()));
};

void ofApp::draw()
{  
  float w = ofGetWidth();
  float h = ofGetHeight();

  if (backend->calib_enabled())
  {
    backend->render_texture(0, 0, w, h);
    backend->render_calib(w, h);
    return;
  }

  syphon_receiver.render_texture(0, 0, w, h);
};

