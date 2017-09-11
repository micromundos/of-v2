#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class CalibGui: public ofBaseApp 
{
  public:

    ofParameter<bool> find_table_plane;
    ofParameter<bool> learn_bg;
    ofParameter<bool> load;
    ofParameter<bool> save;
    ofParameter<bool> log_height_map;
    ofParameter<float> planes_num;
    ofParameter<float> radius_step;
    ofParameter<float> angle_step;

    void init_params()
    {
      p.add( learn_bg.set("1. learn_bg", false) );
      p.add( find_table_plane.set("2. find_table_plane", false) );
      p.add( save.set("3. save", false) );
      p.add( load.set("4. load", false) );

      p.add( log_height_map.set("log_height_map", false) );

      p.add( planes_num.set("planes_num", 20, 0, 100) );
      p.add( radius_step.set("radius_step", 6, 0, 30) );
      p.add( angle_step.set("angle_step", 30, 0, 360) );
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

