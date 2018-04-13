#include "ofApp.h"

void ofApp::inject(shared_ptr<GUI> gui, ofxJSON config, ofxJSON plab_config)
{
  this->gui = gui;
  this->config = config;
  this->plab_config = plab_config;
};

void ofApp::setup()
{
  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);
  ofSetFrameRate(60);
  ofBackground(0);

  flowfield.inject(gui, plab_config);
  particles.inject(&fisica, &flowfield);
  bloques.inject(&fisica, &particles, plab_config);

  backend_client.init(
      config["backend"]["ip"].asString(),
      config["backend"]["port_bin"].asInt(),
      config["backend"]["port_msg"].asInt(),
      config["projector"]["width"].asFloat(), 
      config["projector"]["height"].asFloat(),
      config["calib"]["proj_pts"]);

  fisica.init();

  particles.init(
      config["projector"]["width"].asFloat(), 
      config["projector"]["height"].asFloat());

  flowfield.add(make_shared<FlowFieldContainer>());
  flowfield.add(make_shared<FlowFieldAttractors>());
  flowfield.init(
      plab_config["flow_field"]["width"].asFloat(), 
      plab_config["flow_field"]["height"].asFloat()); 

  bloques.add(make_shared<Emitter>());
  //bloques.add(make_shared<Portal>());
  bloques.init(
      config["projector"]["width"].asFloat(), 
      config["projector"]["height"].asFloat());

  syphon_receiver.init(config["backend"]["syphon"].asString());
  projector_syphon.setName(config["projector"]["syphon"].asString());
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2)); 

  backend_client.update();

  map<int, Bloque> proj_bloques = backend_client.projected_bloques();

  if (!backend_client.juego_active("plab"))
    return;

  bloques.update(proj_bloques);

  if (backend_client.syphon_enabled())
    flowfield.update(syphon_receiver.texture(), proj_bloques);
  else
    flowfield.update(backend_client.projected_pixels(), proj_bloques);

  particles.update();
  fisica.update();
};

void ofApp::draw()
{  
  if (!backend_client.juego_active("plab"))
    return;

  float w = ofGetWidth();
  float h = ofGetHeight(); 

  if (backend_client.render_calib(w, h))
    return;

  if (gui->backend_debug_pixels)
    if (backend_client.syphon_enabled())
      syphon_receiver.render_texture(0, 0, w, h);
    else
      backend_client.render_projected_pixels(w, h);  

  if (gui->flowfield_debug)
    flowfield.render(0, 0, w, h); 

  bloques.render(backend_client.projected_bloques());
  particles.render();

  if (gui->projector_syphon)
    projector_syphon.publishScreen();
};

void ofApp::render_monitor(float w, float h)
{
  if (!backend_client.juego_active("plab"))
    return;

  ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),2), 10, 14, ofColor::yellow, ofColor::black);

  float _w = w/2;
  float _h = h/2; 
  float lh = 24;

  gui->render(0, lh); 

  backend_client.print_connection(0, _h);

  backend_client.print_metadata(0, _h*1.2);
  backend_client.print_bloques(0, _h*1.6);

  if (gui->plab_monitor)
  {
    float mh = h/4;
    syphon_receiver.render_texture(_w, 0, _w, mh);
    flowfield.render_monitor(_w, mh, _w, mh);
  }
};

