#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/utils.h"

int main()
{
  ofxMicromundos::setDataPathRoot();

  cv::FileStorage config = ofxMicromundos::load_config("config.yml");
  cv::FileStorage backend_config = ofxMicromundos::load_config("backend.yml");

  ofGLFWWindowSettings settings;

  //backend
  settings.width = backend_config["monitor"]["width"];
  settings.height = backend_config["monitor"]["height"];
  settings.windowMode = OF_WINDOW; 
  settings.setPosition(ofVec2f(backend_config["monitor"]["x"], backend_config["monitor"]["y"]));
  settings.resizable = false;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> app_win = ofCreateWindow(settings);

  shared_ptr<ofApp> app(new ofApp);

  app->inject(config, backend_config);

  ofRunApp(app_win, app);

  ofRunMainLoop();
}
