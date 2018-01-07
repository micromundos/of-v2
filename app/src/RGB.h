#pragma once

#include "ofMain.h"
#include "GUI.h"
#include "ofxCv.h"

class RGB
{
  public:

    RGB() {};
    ~RGB() 
    {
      dispose();
    };

    void init(shared_ptr<GUI>& gui)
    {
      this->gui = gui;
      vid.setDeviceID(1);
      vid.setDesiredFrameRate(30);
      vid.initGrabber(1920, 1080);
    }; 

    bool update()
    {
      vid.update();
      _updated = vid.isFrameNew();
      return _updated;
      //pix = vid.getPixels();
      //ofxCv::flip(pix, pix, 1); //some cameras need flipping
    };

    void render(float x, float y, float w, float h)
    {
      if (_updated)
        tex.loadData(vid.getPixels());
      if (tex.isAllocated())
        tex.draw(x, y, w, h);
    };

    void dispose()
    {
      vid.close();
      //pix.clear();
      tex.clear();
      gui = nullptr;
    };

    bool updated()
    {
      return _updated;
    };

    ofPixels& pixels()
    {
      return vid.getPixels();
    };

    int width()
    {
      return vid.getPixels().getWidth();
    };

    int height()
    {
      return vid.getPixels().getHeight();
    };

  private:

    shared_ptr<GUI> gui;
    //ofPixels pix;
    ofVideoGrabber vid;
    ofTexture tex;
    bool _updated;

};

