#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/utils.h"
#include "ofxJSON.h"

//#ifdef TARGET_OSX
  //#define micromundos_USE_SYPHON
//#endif

int main()
{
  ofxMicromundos::setDataPathRoot();

  ofxJSON config = ofxMicromundos::load_config("config.json");
  ofxJSON backend_config = ofxMicromundos::load_config("backend.json");

  ofGLFWWindowSettings settings;

  //backend
  settings.width = backend_config["monitor"]["width"].asInt();
  settings.height = backend_config["monitor"]["height"].asInt();
  settings.windowMode = OF_WINDOW; 
  settings.setPosition(ofVec2f(backend_config["monitor"]["x"].asFloat(), backend_config["monitor"]["y"].asFloat()));
  settings.resizable = false;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> app_win = ofCreateWindow(settings);

  shared_ptr<ofApp> app(new ofApp);

  app->inject(config, backend_config);

  ofRunApp(app_win, app);

  ofRunMainLoop();
}
