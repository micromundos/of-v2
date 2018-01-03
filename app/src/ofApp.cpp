#include "ofApp.h"

void ofApp::setup()
{
  ofxMicromundos::setDataPathRoot();
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetWindowPosition(ofGetScreenWidth()-ofGetWidth(), 0);
  ofBackground(0);

  rgb.init(gui);
  chilitags.init(); 
  calib_ready = calib.init();
  seg.init();
};

void ofApp::update()
{
  fps = ofGetFrameRate();
  ofSetWindowTitle(ofToString(fps,2));

  if (!rgb.update())
    return;

  ofPixels& rgb_pix = rgb.pixels();

  chilitags.update(rgb_pix);

  vector<ChiliTag>& tags = chilitags.tags();

  if (!calib_ready)
  {
    calib_ready = calib.update(tags);
    //return;
  }

  seg.update(rgb_pix, tags); 
};

void ofApp::draw()
{ 
  float w = ofGetWidth();
  float h = ofGetHeight();
  //if (!calib_ready)
  if (gui->calib_render)
    calib.render(0, 0, w, h);
  //else
  if (gui->calib_debug)
    calib.debug(seg.pixels(), chilitags.tags(), 0, 0, w, h);
};

