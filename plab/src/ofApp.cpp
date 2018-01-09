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

  //test particles
  float w = ofGetWidth();
  float h = ofGetHeight();
  float x = w/2;
  float y = h/2;
  b2ParticleSystem* b2ps = particles.b2_particles();
  b2Vec2 force(ofRandom(0.5),ofRandom(0.5));
  //force *= 10.;
  ofColor c = ofColor(200, 0, 0);
  for (int i = 0; i < 2000; i++)
  {
    int32 pidx = particles.make_particle(
        ofRandom(x-10,x+10), ofRandom(y-10,y+10), 
        0, 0, c); 
    b2ps->ParticleApplyForce(pidx, force);
  }
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

  backend->update(ofGetWidth(), ofGetHeight());

  //particles.update_flowfield(nullptr, ofVec2f(), ofVec2f()); 
  particles.update();
  fisica.update();
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

