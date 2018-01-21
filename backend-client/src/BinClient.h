#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"

class BinClient
{
  public:

    BinClient() {};
    ~BinClient() {};

    void init(string host, int port)
    {
      client.connect(host, port);
      client.addListener(this);
      received = false;
      locked = false;
    };

    void update(int pix_w, int pix_h, int pix_chan)
    {
      if ( !received )
        return;
      parse(pix_w, pix_h, pix_chan);
      received = false;
      locked = false;
    };

    void render(float x, float y, float w, float h)
    {
      if (tex.isAllocated())
        tex.draw(x, y, w, h);
    };

    void render_info(float x, float y)
    {
      ofxLibwebsockets::Connection* conn = client.getConnection();
      string name = conn->getClientName();
      string ip = conn->getClientIP();
      string info = "bin connected: name="+name + " / ip=" + ip; 
      ofDrawBitmapStringHighlight(info, x, y);
    };

    bool connected()
    {
      return client.isConnected();
    };

    void onMessage( ofxLibwebsockets::Event& args )
    {
      if ( !args.isBinary || locked )
        return;
      pix_data = args.data.getData();
      locked = true;
      received = true;
    }

    void onConnect( ofxLibwebsockets::Event& args )
    {
      ofLogVerbose()<<"bin on connected";
    }

    void onOpen( ofxLibwebsockets::Event& args )
    {
      ofLogVerbose()<<"bin on open";
    }

    void onClose( ofxLibwebsockets::Event& args )
    {
      ofLogVerbose()<<"bin on close";
    }

    void onIdle( ofxLibwebsockets::Event& args )
    {
      ofLogVerbose()<<"bin on idle";
    }

    void onBroadcast( ofxLibwebsockets::Event& args )
    {
      ofLogVerbose()<<"bin got broadcast "<<args.message;
    }

  private:

    ofxLibwebsockets::Client client;
    bool received, locked;

    char* pix_data;
    size_t pix_size;
    ofPixels pix;
    ofTexture tex;

    void parse(int pix_w, int pix_h, int pix_chan)
    {
      if (pix_data == nullptr)
      {
        ofLogWarning() << "pix_data = null";
        return;
      }

      unsigned char* pixd = reinterpret_cast<unsigned char*>(pix_data);
      pix.setFromPixels(pixd, pix_w, pix_h, pix_chan);
      tex.loadData(pix);
    }; 
};

