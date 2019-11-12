#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"
#include "ofxJSON.h"

#include "ofxMicromundos/utils.h"
#include "ofxMicromundos/net/syphon/SyphonReceiver.h"
#include "ofxMicromundos/net/BackendClient.h"

class ofApp : public ofBaseApp
{
  public:
    void setup();
    void draw();

  private:
    SyphonReceiver syphon_receiver;
    SyphonReceiver syphon_backend;
    BackendClient backend_client;
};

