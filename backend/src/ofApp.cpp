#include "ofApp.h"

//#define TIME_MEASUREMENTS_DISABLED

void ofApp::inject(ofxJSON config, ofxJSON backend_config)
{
  this->config = config;
  this->backend_config = backend_config;
};

void ofApp::setup()
{
  TIME_SAMPLE_SET_FRAMERATE(30.0f);
  TIME_SAMPLE_SET_AVERAGE_RATE(0.05);
  ofxTimeMeasurements::instance()->setDrawLocation(TIME_MEASUREMENTS_BOTTOM_LEFT);

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
      backend_config["chilitags"]["fps"].asFloat(),
      backend_config["resize"]["segmentation"].asFloat(),
      backend_config["resize"]["send"].asFloat(),
      config["calib"]["tag_id"].asInt(),
      config["juegos"],
      config["backend"]["port_bin"].asInt(),
      config["backend"]["port_msg"].asInt(),
      config["backend"]["port_blobs"].asInt());

#ifdef micromundos_USE_SYPHON
  syphon_sender
    .init(config["backend"]["syphon"].asString())
    .start();
#endif
};

void ofApp::update()
{
  if (!backend.update())
    return;

  backend.send(
    gui.send_message, 
    gui.send_binary, 
    gui.send_syphon, 
    gui.send_blobs);

#ifdef micromundos_USE_SYPHON
  if (gui.send_syphon)
    syphon_sender.publishTexture(&backend.texture());
#endif
};

void ofApp::draw()
{  
  float w = ofGetWidth();
  float h = ofGetHeight();

  ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),2), 10, 14, ofColor::yellow, ofColor::black);

  float LH = backend.line_height();

  gui.render(0, LH); 

  float _h = h * 0.2;
  float y = _h + LH;
  backend.print_connections(0, y);
  backend.print_metadata(0, y);
  backend.print_blobs(0, y);
  backend.print_bloques(0, y);

  if (gui.backend_monitor)
    backend.render_monitor(w/1.6, 0, w/3, h*0.6);
};

