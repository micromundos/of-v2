#include "ofApp.h"

void ofApp::inject(shared_ptr<GUI> gui, cv::FileStorage config)
{
  this->gui = gui;
  this->config = config;
};

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofBackground(0); 

  float xoff = config["projector_x_offset_from_desktop_width"];
  ofSetWindowPosition(ofGetScreenWidth() + xoff, config["projector_y"]);

  flowfield.inject(gui);
  particles.inject(&fisica, &flowfield);
  bloques.inject(&fisica, &particles, config);

  backend.init(
      config["projector_width"], 
      config["projector_height"],  
      config["cam_width"], 
      config["cam_height"], 
      config["cam_device_id"],
      config["calib_file"],
      config["calib_tag_id"]);

  fisica.init();

  particles.init(
      config["projector_width"], 
      config["projector_height"]);

  flowfield.add(make_shared<FlowFieldContainer>());
  //flowfield.add(make_shared<FlowFieldStream>());
  //flowfield.add(make_shared<FlowFieldAttractors>());
  flowfield.init(
      config["flow_field_width"], 
      config["flow_field_height"]); 

  bloques.add(make_shared<Emitter>());
  //bloques.add(make_shared<Portal>());
  bloques.init(
      config["projector_width"], 
      config["projector_height"]);


  //TODO emit particles c bloque 
  //float w = ofGetWidth();
  //float h = ofGetHeight();
  //float x = w/2;
  //float y = h/2;
  ////b2ParticleSystem* b2ps = particles.b2_particles();
  ////b2Vec2 force(ofRandom(0.5),ofRandom(0.5));
  //////force *= 10.;
  //ofColor c = ofColor(200, 0, 0);
  //float off = w/3;
  //for (int i = 0; i < 2000; i++)
  //{
    //int32 pidx = particles.make_particle(
        //ofRandom(x-off,x+off), ofRandom(y-off,y+off), 
        //0, 0, c); 
    ////b2ps->ParticleApplyForce(pidx, force);
  //}
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

  float w = ofGetWidth();
  float h = ofGetHeight();

  backend.update(w, h);

  ofPixels& proj_pix = backend.projected_pixels();
  map<int, Bloque>& proj_bloques = backend.projected_bloques();

  flowfield.update(proj_pix);
  bloques.update(proj_bloques);

  particles.update();
  fisica.update();
};

void ofApp::draw()
{  
  float w = ofGetWidth();
  float h = ofGetHeight(); 

  if (gui->backend_debug_pixels)
    backend.render_projected_pixels(w, h);

  if (gui->backend_debug_tags)
    backend.render_projected_tags();

  if (gui->flowfield_debug)
    flowfield.render(0, 0, w, h);

  backend.render_calib(w, h);

  bloques.render(backend.projected_bloques());
  particles.render();
};

