#include "ofApp.h"

void ofApp::inject(shared_ptr<GUI> gui, cv::FileStorage config, cv::FileStorage server_config, cv::FileStorage plab_config)
{
  this->gui = gui;
  this->config = config;
  this->server_config = server_config;
  this->plab_config = plab_config;
};

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetFrameRate(60);
  ofBackground(0); 

  ofVec2f proj = ofxMicromundos::projector_position(config);
  ofSetWindowPosition(proj.x, proj.y);

  flowfield.inject(gui);
  particles.inject(&fisica, &flowfield);
  bloques.inject(&fisica, &particles, plab_config);

  backend.init(
      server_config["network"]["ip"],
      server_config["network"]["port_bin"],
      server_config["network"]["port_msg"]);

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

  //TODO BackendClient->calib_enabled: pasar el dato por el socket
  //TODO plab-client: desaparecer del proyector en modo calib
  //if (backend.calib_enabled())
  //{
    //ofSetWindowShape(10, 10);
    //ofSetWindowPosition(0, 0);
    //return;
  //}
  //else
  //{
    //ofSetWindowShape(config["projector"]["width"], config["projector"]["height"]);
    //ofVec2f proj = ofxMicromundos::projector_position(config);
    //ofSetWindowPosition(proj.x, proj.y);
  //}

  if (gui->backend_debug_pixels)
    backend.render_projected_pixels(w, h);  

  if (gui->backend_debug_tags)
    backend.render_projected_tags();

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
  backend.print_info(0, yinfo);
  backend.print_pix_data(0, yinfo + lh*2);

  if (gui->plab_monitor)
    backend.print_bloques(0, yinfo + lh*3);

  gui->render(w/2, mh); 
};

