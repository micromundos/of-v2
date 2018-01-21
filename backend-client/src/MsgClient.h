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

      _pix_w = 0;
      _pix_h = 0;
      _pix_chan = 0;
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
        << " w " << _pix_w
        << " h " << _pix_h 
        << " chan " << _pix_chan;
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

    int pix_width() { return _pix_w; };
    int pix_height() { return _pix_h; };
    int pix_chan() { return _pix_chan; };

    bool pix_ready()
    {
      return _pix_w != 0 && _pix_h != 0 && _pix_chan != 0;
    };

    vector<string>& bloques()
    {
      return _bloques;
    };

  private:

    ofxLibwebsockets::Client client;
    bool received, locked;

    string message;
    vector<string> _bloques;
    int _pix_w, _pix_h, _pix_chan;

    //pixels:dim=640,480#chan=1_bloques:id=0#loc=0,0#dir=0,0#ang=0;id=1#loc=1,1#dir=1,1#ang=1
    void parse()
    {
      vector<string> data = ofSplitString(message, "_");

      if (data.size() > 0)
      {
        vector<string> p0 = ofSplitString(data[0], ":");
        if (p0.size() > 1)
        {
          vector<string> pix_metadata = ofSplitString(p0[1], "#");
          vector<string> pix_dim = ofSplitString(ofSplitString(pix_metadata[0], "=")[1], ",");
          _pix_w = stoi(pix_dim[0]);
          _pix_h = stoi(pix_dim[1]);
          _pix_chan = stoi(ofSplitString(pix_metadata[1], "=")[1]);
        }
      }

      if (data.size() > 1)
      {
        _bloques = ofSplitString(ofSplitString(data[1], ":")[1], ";");
      }
    };
};

