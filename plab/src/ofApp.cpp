#include "ofApp.h"

void ofApp::inject(shared_ptr<GUI> gui, cv::FileStorage config, cv::FileStorage backend_config, cv::FileStorage plab_config)
{
  this->gui = gui;
  this->config = config;
  this->backend_config = backend_config;
  this->plab_config = plab_config;
};

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetFrameRate(60);

  projector = ofxMicromundos::projector(true, config);

  flowfield.inject(gui);
  particles.inject(&fisica, &flowfield);
  bloques.inject(&fisica, &particles, plab_config);

  backend.init(
      backend_config["network"]["ip"],
      backend_config["network"]["port_bin"],
      backend_config["network"]["port_msg"],
      config["projector"]["width"], 
      config["projector"]["height"],
      config["calib"]["proj_pts"]);

  fisica.init();

  particles.init(
      config["projector"]["width"], 
      config["projector"]["height"]);

  flowfield.add(make_shared<FlowFieldContainer>());
  //flowfield.add(make_shared<FlowFieldStream>());
  //flowfield.add(make_shared<FlowFieldAttractors>());
  flowfield.init(
      plab_config["flow_field"]["width"], 
      plab_config["flow_field"]["height"]); 

  bloques.add(make_shared<Emitter>());
  //bloques.add(make_shared<Portal>());
  bloques.init(
      config["projector"]["width"], 
      config["projector"]["height"]);
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

  backend.update();

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

  if (backend.render_calib(w, h))
    return;

  if (gui->backend_debug_pixels)
    backend.render_projected_pixels(w, h);  

  if (gui->flowfield_debug)
    flowfield.render(0, 0, w, h); 

  bloques.render(backend.projected_bloques());
  particles.render();
};

void ofApp::render_monitor(float w, float h)
{
  float mh = h*0.7;

  if (gui->plab_monitor)
    flowfield.render_monitor(0, 0, w, mh);

  float lh = 24;
  float yinfo = mh;
  backend.print_connection(0, yinfo);
  backend.print_metadata(0, yinfo + lh*2);

  if (gui->plab_monitor)
    backend.print_bloques(0, yinfo + lh*3);

  gui->render(w/2, mh); 
};

