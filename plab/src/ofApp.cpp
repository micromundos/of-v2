#include "ofApp.h"

void ofApp::inject(shared_ptr<GUI> gui, cv::FileStorage config, cv::FileStorage plab_config)
{
  this->gui = gui;
  this->config = config;
  this->plab_config = plab_config;
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
  bloques.inject(&fisica, &particles, plab_config);

  backend.init(
      config["projector_width"], 
      config["projector_height"],  
      config["cam_width"], 
      config["cam_height"], 
      config["cam_device_id"],
      config["calib_file"],
      config["calib_tag_id"],
      config["calib_tags_size"]);

  fisica.init();

  particles.init(
      config["projector_width"], 
      config["projector_height"]);

  flowfield.add(make_shared<FlowFieldContainer>());
  //flowfield.add(make_shared<FlowFieldStream>());
  //flowfield.add(make_shared<FlowFieldAttractors>());
  flowfield.init(
      plab_config["flow_field_width"], 
      plab_config["flow_field_height"]); 

  bloques.add(make_shared<Emitter>());
  //bloques.add(make_shared<Portal>());
  bloques.init(
      config["projector_width"], 
      config["projector_height"]);
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

  bool updated = backend.update();

  if (updated)
  {
    ofPixels& proj_pix = backend.projected_pixels();
    map<int, Bloque>& proj_bloques = backend.projected_bloques();

    flowfield.update(proj_pix);
    bloques.update(proj_bloques);
  }

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

  if (backend.render_calib(w, h))
    return;

  if (gui->flowfield_debug)
    flowfield.render(0, 0, w, h); 

  bloques.render(backend.projected_bloques());
  particles.render();
};
