#include "ofApp.h"

void ofApp::inject(shared_ptr<GUI> gui)
{
  this->gui = gui;
};

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetWindowPosition(ofGetScreenWidth()-ofGetWidth(), 0);
  ofBackground(0);

  particles.inject(&fisica);
  flowfield.inject(gui);

  backend.init(ofGetWidth(), ofGetHeight());
  flowfield.init(320, 240); //TODO flow field size from settings
  fisica.init();
  particles.init();

  //TODO emit particles c bloque
  float w = ofGetWidth();
  float h = ofGetHeight();
  float x = w/2;
  float y = h/2;
  //b2ParticleSystem* b2ps = particles.b2_particles();
  //b2Vec2 force(ofRandom(0.5),ofRandom(0.5));
  ////force *= 10.;
  ofColor c = ofColor(200, 0, 0);
  float off = w/3;
  for (int i = 0; i < 2000; i++)
  {
    int32 pidx = particles.make_particle(
        ofRandom(x-off,x+off), ofRandom(y-off,y+off), 
        0, 0, c); 
    //b2ps->ParticleApplyForce(pidx, force);
  }
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

  float w = ofGetWidth();
  float h = ofGetHeight();

  backend.update(ofGetWidth(), ofGetHeight());
  flowfield.update(backend.projected_pixels());

  particles.update_flowfield(flowfield.get(), w, h, flowfield.width(), flowfield.height()); 
  particles.update();
  fisica.update();
};

void ofApp::draw()
{  
  float w = ofGetWidth();
  float h = ofGetHeight();

  if (gui->backend_debug)
    backend.render_projected(w, h);

  backend.render_calib(w, h);

  particles.render();
};

