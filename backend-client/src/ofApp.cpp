#include "ofApp.h"


void ofApp::setup(){

  ofSetLogLevel(OF_LOG_NOTICE);
  ofBackground(40);
  ofSetFrameRate(30);

  fullscreen = false;
  ofSetFullscreen(false);
  ofSetWindowPosition(300, 0);

  msg.init("localhost", 9998); 
  bin.init("localhost", 9999);

  //tcp.setup("127.0.0.1", 9999);
}


void ofApp::update(){

  msg.update(); 

  if (msg.pix_ready())
    bin.update(msg.pix_width(), msg.pix_height(), msg.pix_chan());

  //if (!tcp.isConnected())
    //return;
  //parse_message(tcp.receive()); 
  //int received = tcp.receiveRawBytes(pix_data, 786432);
  //int received = tcp.receiveRawBytes(pix_data, pix_size);
  //ofLog() << "received bytes " << received;
  //parse_binary();
}


void ofApp::draw(){

  float x = 6;
  float y = 20;

  if (!bin.connected())
  {
    ofDrawBitmapStringHighlight("bin not connected", x, y, ofColor::red, ofColor::black);
    return;
  }

  if (fullscreen)
  {
    bin.render(0, 0, ofGetWidth(), ofGetHeight());
    return;
  }

  float lh = 24; 

  bin.print_connection(x, y);
  y += lh;

  msg.print_connection(x, y);
  y += lh;

  y += lh/2;
  ofDrawBitmapStringHighlight("f = fullscreen", x, y);
  y += lh;

  float w = ((float)ofGetWidth())/2;
  float h = w*(3./4);
  bin.render(x, y, w, h);
  y += h + lh;

  msg.print_metadata(x, y);
  y += lh;

  msg.print_bloques(x, y);
}


void ofApp::keyPressed(int key){

}


void ofApp::keyReleased(int key){

  if (key == 'f')
  {
    fullscreen = !fullscreen;
    ofSetFullscreen(fullscreen);
    ofSetWindowShape(fullscreen ? 1024 : 600, fullscreen ? 768 : 600);
    //ofSetWindowPosition(fullscreen ? ofGetScreenWidth() : 300, 0);
  }

}


void ofApp::mouseMoved(int x, int y ){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

}


void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::mouseEntered(int x, int y){

}


void ofApp::mouseExited(int x, int y){

}


void ofApp::windowResized(int w, int h){

}


void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
