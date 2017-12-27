#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class CalibGui: public ofBaseApp 
{
  public:

    ofParameter<bool> bulean;
    ofParameter<float> flout;

    void init_params()
    {
      p.add( bulean.set("bulean", false) );
      p.add( flout.set("height_map_threshold", 0.5, 0., 1.) );
    };

    void setup()
    {
      p.setName("params");
      init_params();
      gui.setup(p);
      //ofBackground(0);
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

