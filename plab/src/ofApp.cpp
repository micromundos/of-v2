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
  ofSetFrameRate(30);
  ofBackground(0);

  //TODO handle projector/minimize on ofxMicromundos::Juego
  ofxMicromundos::projector(config);

  float proj_w = config["projector"]["width"].asFloat();
  float proj_h = config["projector"]["height"].asFloat();

  gaussian
    .init(proj_w, proj_h)
    .on("update", this, &ofApp::update_gaussian);

  plab.inject(gui, plab_config);

  backend_client.init(
      config["backend"]["ip"].asString(),
      config["backend"]["port_bin"].asInt(),
      config["backend"]["port_msg"].asInt(),
      config["backend"]["port_blobs"].asInt(),
      proj_w, 
      proj_h);

  plab.init(proj_w, proj_h);

#ifdef micromundos_USE_SYPHON
  syphon_backend.init(config["backend"]["syphon"].asString());
  syphon_projector.init(config["projector"]["syphon"].asString());
#endif
};

void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2)); 

  backend_client.update();

#ifdef micromundos_USE_SYPHON
  if (!backend_client.juego_active("plab"))
    syphon_projector.stop();
  else if (!syphon_projector.running() && gui->syphon_projector)
    syphon_projector.start();
#endif 

  if (!backend_client.juego_active("plab"))
    return;

#ifdef micromundos_USE_SYPHON
  if (backend_client.syphon_enabled())
    plab.update(
      syphon_backend.texture(), 
      backend_client.bloques());
  else
#endif
    plab.update(
      backend_client.pixels(), 
      backend_client.bloques());
};

void ofApp::draw()
{  
  if (!backend_client.juego_active("plab"))
    return;

  float w = ofGetWidth();
  float h = ofGetHeight();

  if (backend_client.calib_enabled())
  {
    backend_client.render_texture(0, 0, w, h);
    backend_client.render_calib();
    return;
  } 

  if (gui->flowfield_debug)
    plab.render_debug(0, 0, w, h);

  if (gui->render_backend_tex)
    render_backend_tex(w, h, gui->blur_backend_tex);

  if (gui->render_blobs)
    render_blobs(w, h);

  plab.render();

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

void ofApp::render_backend_tex(float w, float h, bool blur)
{
  if (!blur)
  {
#ifdef micromundos_USE_SYPHON
    if (backend_client.syphon_enabled())
      syphon_backend.render_texture(0, 0, w, h);
    else
#endif
      backend_client.render_texture(0, 0, w, h);
    return;
  }

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

    plab.render_monitor(_w, mh, _w, mh);
  }
};

