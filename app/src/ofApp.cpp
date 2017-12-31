#include "ofApp.h"

void ofApp::setup()
{
  ofxMicromundos::setDataPathRoot();
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);

  rgb.init(gui);
  chilitags.init(); 
  calib_ready = calib.init();
  seg.init();
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

  if (!rgb.update())
    return;

  ofPixels& rgb_pix = rgb.pixels();

  chilitags.update(rgb_pix);

  vector<ChiliTag>& tags = chilitags.tags();

  if (!calib_ready)
  {
    calib_ready = calib.update(tags);
    return;
  }

  seg.update(rgb_pix, tags); 
};

void ofApp::draw()
{
  float w = ofGetWidth();
  float h = ofGetHeight();

  float hw = w/2;
  float hh = h/2;

  //top left
  rgb.render( 0, 0, hw, hh );
  chilitags.render( 0, 0, hw, hh );
  //calib.render( chilitags.tags(), 0, 0, hw, hh );

  //top right
  seg.render( hw, 0, hw, hh );

  //TODO test calibration: render seg img to projector
  //fbo.begin();
  //seg.render( 0, 0, w, h );
  //fbo.end();
  //glPushMatrix();
  //glMultMatrixf(calib.homography());
  //fbo.draw(0, 0);
  //glPopMatrix();
};

