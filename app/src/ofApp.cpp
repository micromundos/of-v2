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

  seg.update(rgb_pix, tags); 

  if (!calib_ready)
  {
    calib_ready = calib.find(tags);
    //return;
  }

  calib.transform(rgb_pix, proj_pix);
  calib.transform(tags, proj_tags);
};

void ofApp::draw()
{  
  //if (!calib_ready)
  if (gui->calib_render)
  {
    render_proj_pix();
    render_proj_tags();
    calib.render();
  }
};

void ofApp::render_proj_pix()
{
  proj_tex.loadData(proj_pix);
  proj_tex.draw(0, 0, ofGetWidth(), ofGetHeight());
};

void ofApp::render_proj_tags()
{
  ofPushStyle();
  ofSetColor(ofColor::magenta);
  for (int i = 0; i < proj_tags.size(); i++)
  {
    vector<ofVec2f> &corners = proj_tags[i].corners;
    ofVec2f p0, p1;
    for (int j = 0; j < corners.size(); j++)
    {
      p0 = corners[j];
      p1 = corners[ (j+1)%4 ]; 
      ofDrawLine( p0.x, p0.y, p1.x, p1.y );
    }
  }
  ofPopStyle();
};

