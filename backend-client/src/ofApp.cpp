#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

  ofBackground(40);
  tcpClient.setup("127.0.0.1", 9999);
	//tcpClient.setMessageDelimiter(",");
}

//--------------------------------------------------------------
void ofApp::update(){

  if (!tcpClient.isConnected())
    return;

  bloques = ofSplitString(tcpClient.receive(), ";");
  //tcpClient.receiveRawBytes(char *receiveBytes, int numBytes);
}

//--------------------------------------------------------------
void ofApp::draw(){

  float x = 6;
  float y = 20;

  if (!tcpClient.isConnected())
  {
    ofDrawBitmapStringHighlight("not connected", x, y, ofColor::red, ofColor::black);
    return;
  }

  string info = "connected: port: "+ofToString(tcpClient.getPort()) + " ip: " + tcpClient.getIP(); 
  ofDrawBitmapStringHighlight(info, x, y, ofColor::green, ofColor::black);

  ofDrawBitmapStringHighlight("bloques", x, y+24);
  for (int i = 0; i < bloques.size(); i++)
    ofDrawBitmapStringHighlight(bloques[i], x, y+24*2 + 20*i);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
