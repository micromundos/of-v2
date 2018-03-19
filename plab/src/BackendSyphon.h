#pragma once

#include "ofxSyphon.h"

class BackendSyphon
{
  public:

    BackendSyphon() {};
    ~BackendSyphon() {};

    void init(string server_name, string app_name)
    {
      client.setup();
      client.set(server_name, app_name);
    };

    ofTexture& projected_texture()
    {
      client.bind();
      int w = client.getWidth();
      int h = client.getHeight();
      if (w == 0 || h == 0)
        return tex;
      tex = client.getTexture();
      client.unbind();
      return tex;
    };

    void render_projected_texture(float x, float y, float w, float h)
    {
      ofTexture& tex = projected_texture();
      if (tex.isAllocated())
        tex.draw(x, y, w, h);
    };

    //TODO FIXME BackendSyphon projected_pixels
    //ofPixels& projected_pixels()
    //{
      //client.bind();
      //int w = client.getWidth();
      //int h = client.getHeight();
      ////client.unbind();

      //if (w == 0 || h == 0)
        //return pix;

      ////client.getTexture().readToPixels(pix);
      ////pix.allocate(w, h, 4);

      //if (!fbo.isAllocated())
      //{
        //ofFbo::Settings s;
        //s.width = w;
        //s.height = h;
        //s.internalformat = GL_RGBA; //GL_RGBA32F_ARB; 
        //s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        //s.minFilter = GL_NEAREST;
        //s.maxFilter = GL_NEAREST;
        //s.wrapModeHorizontal = GL_CLAMP;
        //s.wrapModeVertical = GL_CLAMP;
        //fbo.allocate(s); 
        ////fbo.allocate(w, h); 
      //}

      //fbo.begin();
      //ofClear(0,255);
      //client.getTexture().draw(0, 0, w, h);
      ////client.draw(0, 0, w, h);
      //client.unbind();
      //fbo.end();
      //fbo.readToPixels(pix);

      //return pix;
    //}; 

  private:

    ofxSyphonClient client;
    ofTexture tex;
    //ofPixels pix;
    //ofFbo fbo;
};

