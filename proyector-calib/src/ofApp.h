#pragma once

#include "ofMain.h"

#include "ofxJSON.h"

#include "ofxMicromundos/utils.h"
#include "ofxMicromundos/net/BackendClient.h"

class ofApp : public ofBaseApp
{
  public:
    void setup();
    void update();
    void draw();

  private:
    BackendClient backend_client;
};

