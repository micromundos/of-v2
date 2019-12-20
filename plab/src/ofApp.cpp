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

#ifndef micromundos_USE_SYPHON
  ofxMicromundos::projector(config);
#endif

  float proj_w = config["projector"]["width"].asFloat();
  float proj_h = config["projector"]["height"].asFloat();

  gaussian
    .init(proj_w, proj_h)
    .on("update", this, &ofApp::update_gaussian);

  flowfield.inject(gui, &bloques);
  particles.inject(&fisica);
  bloques.inject(&fisica, &particles, plab_config);

  backend_client.init(
      config["backend"]["ip"].asString(),
      config["backend"]["port_bin"].asInt(),
      config["backend"]["port_msg"].asInt(),
      config["backend"]["port_blobs"].asInt(),
      proj_w, 
      proj_h);

  fisica.init();
  particles.init(proj_w, proj_h);

  flowfield.add(make_shared<Container>());
  flowfield.add(make_shared<Attractors>("attractor",1));
  flowfield.add(make_shared<Attractors>("repulsor",-1));
  //flowfield.add(make_shared<Transporter>());

  flowfield.init(
      plab_config["flow_field"]["width"].asFloat(), 
      plab_config["flow_field"]["height"].asFloat(),
      proj_w, 
      proj_h); 

  bloques.add(make_shared<Emitter>());
  //bloques.add(make_shared<Portal>());
  bloques.init(proj_w, proj_h);

#ifdef micromundos_USE_SYPHON
  syphon_backend.init(config["backend"]["syphon"].asString());
  syphon_projector.init(config["projector"]["syphon"].asString());
#endif
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2)); 

  backend_client.update();

  if (!backend_client.juego_active("plab"))
    return;

  bloques.update(backend_client.bloques());

#ifdef micromundos_USE_SYPHON
  if (backend_client.syphon_enabled())
    flowfield.update(syphon_backend.texture());
  else
#endif
    flowfield.update(backend_client.pixels());

  particles.update(flowfield.get(), flowfield.width(), flowfield.height(), flowfield.channels());
  fisica.update();
};

void ofApp::draw()
{  
#ifdef micromundos_USE_SYPHON
  if (!backend_client.juego_active("plab"))
  {
    syphon_projector.stop();
    return;
  }
  else if (!syphon_projector.running() && gui->syphon_projector)
  {
    syphon_projector.start();
  } 
#endif

  float w = ofGetWidth();
  float h = ofGetHeight();

  if (backend_client.calib_enabled())
  {
    backend_client.render_texture(0, 0, w, h);
    backend_client.render_calib();
    return;
  } 

  render_debug(w, h); 

  if (gui->render_backend_tex)
    render_backend_tex(w, h);

  if (gui->render_blobs)
    render_blobs(w, h);

  flowfield.render(); 
  bloques.render();
  particles.render();

#ifdef micromundos_USE_SYPHON
  if (gui->syphon_projector)
    syphon_projector.publishScreen();
#endif
};

void ofApp::exit()
{
  gaussian
    .off("update", this, &ofApp::update_gaussian)
    .dispose();
};

void ofApp::update_gaussian(ofShader& shader)
{
  shader.setUniform1f("sigma", gui->gaussian_sigma);
  shader.setUniform1i("kernel", gui->gaussian_kernel);
  shader.setUniform1f("alpha", 1.0);
};

void ofApp::render_backend_tex(float w, float h)
{
  ofTexture* tex;
#ifdef micromundos_USE_SYPHON
  if (backend_client.syphon_enabled())
    tex = &(syphon_backend.texture());
  else
#endif
    tex = &(backend_client.texture());

  if (tex->isAllocated())
  {
    gaussian
      .set("data", *tex)
      .update(2) //horiz + vert
      .get()
      .draw(0, 0, w, h);
  }
};

void ofApp::render_blobs(float w, float h)
{
  ofPushStyle();
  ofSetColor(ofColor::white);
  ofSetLineWidth(4);
  backend_client.render_blobs(0, 0, w, h);
  ofPopStyle();
};

void ofApp::render_debug(float w, float h)
{  
  if (gui->flowfield_debug)
    flowfield.render_debug(0, 0, w, h);
};

void ofApp::render_monitor(float w, float h)
{
  if (!backend_client.juego_active("plab"))
    return;

  ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),2), 10, 14, ofColor::yellow, ofColor::black);

  float _w = w/2;
  float _h = h/2; 
  float LH = backend_client.line_height();

  gui->render(0, LH); 

  float y = _h + LH;
  backend_client.print_connections(0, y);
  backend_client.print_metadata(0, y);
  backend_client.print_bloques(0, y);

  if (gui->plab_monitor)
  {
    float mh = h/4;

#ifdef micromundos_USE_SYPHON
    if (backend_client.syphon_enabled())
      syphon_backend.render_texture(_w, 0, _w, mh);
    else
#endif
      backend_client.render_texture(_w, 0, _w, mh);

    flowfield.render_monitor(_w, mh, _w, mh);
  }
};

