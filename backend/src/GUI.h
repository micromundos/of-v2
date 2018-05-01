#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GUI
{
  public:

    ofParameter<bool> backend_monitor;
    ofParameter<bool> send_message;
    ofParameter<bool> send_binary;
    ofParameter<bool> send_syphon;
    ofParameter<bool> send_blobs;

    void init_params()
    {
      p.add( backend_monitor
          .set("backend_monitor", true) ); 

      p.add( send_message
          .set("send_message", true) ); 

      p.add( send_binary
          .set("send_binary", true) ); 

      p.add( send_syphon
          .set("send_syphon", false) ); 

      p.add( send_blobs
          .set("send_blobs", true) );
    }; 

    void init(float w)
    {
      ofxBaseGui::setDefaultWidth(w);
      p.setName("params");
      init_params();
      gui.setup(p, "backend_gui.xml");
      gui.loadFromFile("backend_gui.xml");
    }; 

    void render(float x, float y)
    { 
      gui.setPosition(x, y);
      gui.draw();
    };

  private:

    ofParameterGroup p;
    ofxPanel gui; 
};

