#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

  ofSetLogLevel(OF_LOG_NOTICE);
  ofBackground(40);
  ofSetFrameRate(60);

  fullscreen = false;
  ofSetFullscreen(false);
  ofSetWindowPosition(300, 0);

  ws.connect("localhost", 9999);
  ws.addListener(this);

  bin_received = false;
  bin_locked = false;
  msg_received = false;
  msg_locked = false; 

  //tcp.setup("127.0.0.1", 9999);
}

//--------------------------------------------------------------
void ofApp::update(){

  if ( bin_received )
  {
    parse_binary();
    bin_received = false;
    bin_locked = false;
  }

  if ( msg_received )
  {
    parse_message();
    msg_received = false;
    msg_locked = false;
  }

  //if (!tcp.isConnected())
    //return;
  //parse_message(tcp.receive()); 
  //int received = tcp.receiveRawBytes(pix_data, 786432);
  //int received = tcp.receiveRawBytes(pix_data, pix_size);
  //ofLog() << "received bytes " << received;
  //parse_binary();
}

//--------------------------------------------------------------
void ofApp::draw(){

  float x = 6;
  float y = 20;

  if (!ws.isConnected())
  {
    ofDrawBitmapStringHighlight("not connected", x, y, ofColor::red, ofColor::black);
    return;
  }

  if (fullscreen)
  {
    if (tex.isAllocated())
      tex.draw(0, 0, ofGetWidth(), ofGetHeight());
    return;
  }

  ofxLibwebsockets::Connection* conn = ws.getConnection();
  string name = conn->getClientName();
  string ip = conn->getClientIP();
  string info = "connected: name "+name + " ip " + ip; 
  ofDrawBitmapStringHighlight(info, x, y, ofColor::green, ofColor::black);

  y += 24;
  ofDrawBitmapStringHighlight("bloques", x, y);

  y += 24;
  for (int i = 0; i < bloques.size(); i++)
  {
    ofDrawBitmapStringHighlight(bloques[i], x, y);
    y += 20;
  }

  float w = ofGetWidth();
  if (tex.isAllocated())
    tex.draw(x, y, w/2, (w/2)*(3./4));
}

void ofApp::parse_binary()
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

//pixels:size=307200#dim=640,480#chan=1_bloques:id=0#loc=0,0#dir=0,0#ang=0;id=1#loc=1,1#dir=1,1#ang=1
void ofApp::parse_message()
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


//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){

  // need to load this next frame!
  if ( args.isBinary && !bin_locked )
  {
    ofLog() << "binary msg size " << args.data.size() << " -- pix size " << pix_size;
    pix_data = args.data.getData();
    bin_locked = true;
    bin_received = true;
  } 

  else if (!msg_locked)
  {
    ofLog() << "string msg " << args.message;
    message = args.message;
    msg_locked = true;
    msg_received = true;
  }
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on connected";
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on open";
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on close";
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    ofLogVerbose()<<"on idle";
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;    
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

  if (key == 'f')
  {
    fullscreen = !fullscreen;
    ofSetFullscreen(fullscreen);
    ofSetWindowPosition(fullscreen ? ofGetScreenWidth() : 300, 0);
  }

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
