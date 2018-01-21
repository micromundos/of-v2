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

    void update()
    {
      if ( !received )
        return;
      parse();
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
    ofPixels pix;
    ofTexture tex;

    void parse()
    {
      if (pix_data == nullptr)
      {
        ofLogWarning() << "pix_data = null";
        return;
      }

      unsigned char* pix_data2 = reinterpret_cast<unsigned char*>(pix_data);
      pix.setFromPixels(pix_data2, 1024, 768, 1);
      //pix.setFromPixels(pix_data2, pix_w, pix_h, pix_chan);
      tex.loadData(pix);
    }; 
};

