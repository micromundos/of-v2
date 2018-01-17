#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GUI: public ofBaseApp 
{
  public:

    ofParameter<bool> backend_monitor;
    ofParameter<bool> debug_tags;
    ofParameter<bool> debug_pixels;

    ofParameter<float> container_force_mult;
    ofParameter<float> edge_threshold;
    ofParameter<float> gaussian_kernel;
    ofParameter<float> gaussian_sigma;

    void init_params()
    {
      p.add( backend_monitor
          .set("backend_monitor", true) );

      p.add( debug_tags
          .set("debug_tags", true) );

      p.add( debug_pixels
          .set("debug_pixels", true) );

      p.add( container_force_mult
          .set("container_force_mult", 1., 0., 10.) );

      p.add( edge_threshold
          .set("edge_threshold", 1., 0., 10.) );

      p.add( gaussian_kernel
          .set("gaussian_kernel", 3., 0., 10.) );

      p.add( gaussian_sigma
          .set("gaussian_sigma", 5., 0., 20.) );
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

