#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"
#include "ofxJSON.h"

#include "ofxMicromundos/utils.h"
#include "ofxMicromundos/SyphonReceiver.h"
#include "ofxMicromundos/BackendClient.h"

class ofApp : public ofBaseApp
{
  public:
    void setup();
    void draw();

  private:
    SyphonReceiver syphon_receiver;
    BackendClient backend_client;
};

