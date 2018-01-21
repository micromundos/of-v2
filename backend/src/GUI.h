#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GUI //: public ofBaseApp 
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

    void init(float w)
    {
      ofxBaseGui::setDefaultWidth(w);
      p.setName("params");
      init_params();
      gui.setup(p, "server_gui.xml");
      gui.loadFromFile("server_gui.xml");
    }; 

    void render(float x, float y)
    { 
      gui.setPosition(x, y);
      gui.draw();
    };

    //void setup()
    //{
      //ofSetFrameRate(30);
      //ofBackground(40);
      //ofSetVerticalSync(false);
      //init(ofGetWidth());
    //};

    //void draw()
    //{ 
      //render(0, 0);
    //};

    //void update() {}; 

  private:

    ofParameterGroup p;
    ofxPanel gui; 
};

