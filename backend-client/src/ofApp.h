#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"
//#include "ofxNetwork.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//ofxTCPClient tcp;
    ofxLibwebsockets::Client ws;

    bool fullscreen;

    char* pix_data;
    int pix_w, pix_h, pix_chan, pix_size;
    string message;
    vector<string> bloques; 

    bool bin_received, bin_locked;
    bool msg_received, msg_locked;

    void parse_message();
    void parse_binary();

    ofPixels pix;
    ofTexture tex;

    // websocket 
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
};
