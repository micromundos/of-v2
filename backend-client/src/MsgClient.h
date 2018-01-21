#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"

class MsgClient
{
  public:

    MsgClient() {};
    ~MsgClient() {};

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

    void render_info(float x, float y)
    {
      ofxLibwebsockets::Connection* conn = client.getConnection();
      string name = conn->getClientName();
      string ip = conn->getClientIP();
      string info = "msg connected: name="+name + " / ip=" + ip; 
      ofDrawBitmapStringHighlight(info, x, y);
    };

    void render_pix_data(float x, float y)
    {
      stringstream info;
      info << "msg pix" 
        << " w " << pix_w
        << " h " << pix_h 
        << " chan " << pix_chan
        << " size " << pix_size;
      ofDrawBitmapStringHighlight(info.str(), x, y);
    };

    bool connected()
    {
      return client.isConnected();
    };

    void onMessage( ofxLibwebsockets::Event& args )
    {
      if ( args.isBinary || locked )
        return;
      message = args.message;
      locked = true;
      received = true;
    }

    void onConnect( ofxLibwebsockets::Event& args )
    {
      ofLogVerbose()<<"msg on connected";
    }

    void onOpen( ofxLibwebsockets::Event& args )
    {
      ofLogVerbose()<<"msg on open";
    }

    void onClose( ofxLibwebsockets::Event& args )
    {
      ofLogVerbose()<<"msg on close";
    }

    void onIdle( ofxLibwebsockets::Event& args )
    {
      ofLogVerbose()<<"msg on idle";
    }

    void onBroadcast( ofxLibwebsockets::Event& args )
    {
      ofLogVerbose()<<"msg got broadcast "<<args.message;
    }

    vector<string>& get_bloques()
    {
      return bloques;
    };

  private:

    ofxLibwebsockets::Client client;
    bool received, locked;

    string message;
    vector<string> bloques;
    int pix_w, pix_h, pix_chan, pix_size;

    //pixels:size=307200#dim=640,480#chan=1_bloques:id=0#loc=0,0#dir=0,0#ang=0;id=1#loc=1,1#dir=1,1#ang=1
    void parse()
    {
      vector<string> data = ofSplitString(message, "_");

      if (data.size() > 0)
      {
        vector<string> p0 = ofSplitString(data[0], ":");
        if (p0.size() > 1)
        {
          vector<string> pix_metadata = ofSplitString(p0[1], "#");
          pix_size = stoi(ofSplitString(pix_metadata[0], "=")[1]);
          vector<string> pix_dim = ofSplitString(ofSplitString(pix_metadata[1], "=")[1], ",");
          pix_w = stoi(pix_dim[0]);
          pix_h = stoi(pix_dim[1]);
          pix_chan = stoi(ofSplitString(pix_metadata[2], "=")[1]);
        }
      }

      if (data.size() > 1)
      {
        bloques = ofSplitString(ofSplitString(data[1], ":")[1], ";");
      }
    };
};

