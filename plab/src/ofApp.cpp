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

  particles.inject(&fisica);
  backend->init(ofGetWidth(), ofGetHeight());
  fisica.init();
  particles.init();

  float w = ofGetWidth();
  float h = ofGetHeight();
  b2ParticleSystem* b2ps = particles.b2_particles();
  b2Vec2 force(1,1);
  force *= 100.;
  ofColor c = ofColor(200, 0, 0);
  for (int i = 0; i < 100; i++)
  {
    int32 pidx = particles.make_particle(w/2, h/2, 10, 10, c); 
    b2ps->ParticleApplyForce(pidx, force);
  }
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
  backend->update(ofGetWidth(), ofGetHeight());
  fisica.update();
  particles.update();
};

void ofApp::draw()
{  
  float w = ofGetWidth();
  float h = ofGetHeight();

  backend->render_calib(w, h);

  if (gui->backend_debug)
    backend->render_projected(w, h);

  particles.render();
};

