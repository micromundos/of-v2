#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GUI: public ofBaseApp 
{
  public:

    ofParameter<bool> backend_monitor;
    ofParameter<bool> send;

    void init_params()
    {
      p.add( backend_monitor
          .set("backend_monitor", true) );

      p.add( send
          .set("send", true) );
    };

    void setup()
    {
      ofSetFrameRate(30);
      ofBackground(40);
      p.setName("params");
      init_params();
      gui.setup(p, "server_gui.xml");
      gui.loadFromFile("server_gui.xml");
      ofSetVerticalSync(false);
    };

    void update()
    {};

    void draw()
    {
      gui.draw();
    }; 

  private:

    ofParameterGroup p;
    ofxPanel gui; 
};

