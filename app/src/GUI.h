#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GUI: public ofBaseApp 
{
  public:

    ofParameter<bool> backend_monitor;
    ofParameter<bool> backend_debug_tags;
    ofParameter<bool> backend_debug_pixels;
    //ofParameter<float> flout;

    void init_params()
    {
      p.add( backend_monitor
          .set("backend_monitor", true) );

      p.add( backend_debug_tags
          .set("backend_debug_tags", true) );

      p.add( backend_debug_pixels
          .set("backend_debug_pixels", true) );

      //p.add( flout.set("flout", 0.5, 0., 1.) );
    };

    void setup()
    {
      p.setName("params");
      init_params();
      gui.setup(p, "gui.xml");
      gui.loadFromFile("gui.xml");
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

