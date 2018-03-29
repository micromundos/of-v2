#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"
#include "opencv2/opencv.hpp"

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

