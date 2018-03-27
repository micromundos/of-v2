#include "ofMain.h"
#include "ofApp.h"

int main( )
{
  ofxMicromundos::setDataPathRoot();
  ofSetupOpenGL(10,10,OF_WINDOW);
  ofRunApp(new ofApp());
}
