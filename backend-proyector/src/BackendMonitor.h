#pragma once

#include "ofMain.h"
#include "ofxMicromundos/GUI.h"

class BackendMonitor : public ofBaseApp 
{
  public: 

    void inject(shared_ptr<Backend> backend, shared_ptr<GUI> backend_gui)
    {
      this->backend = backend;
      this->backend_gui = backend_gui;
    };

    void setup() 
    {
      ofSetFrameRate(30);
      ofBackground(40);
      backend_gui->init((float)ofGetWidth()/2);
    };

    void draw()
    {
      float w = ofGetWidth();
      float h = ofGetHeight();

      ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(),2), 10, 14, ofColor::yellow, ofColor::black);

      float _h = h/3;  
      float LH = backend->line_height();

      backend_gui->render(0, LH); 

      float y = _h + LH;
      backend->print_connections(0, y);
      backend->print_metadata(0, y);
      backend->print_blobs(0, y);
      backend->print_bloques(0, y);

      if (backend_gui->backend_monitor)
        backend->render_monitor(w/2, 0, w/3, h/2);
    };

  private:

    shared_ptr<Backend> backend;
    shared_ptr<GUI> backend_gui;
};

