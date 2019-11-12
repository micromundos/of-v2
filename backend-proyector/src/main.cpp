#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/GUI.h"
#include "ofxMicromundos/utils.h"
#include "ofxJSON.h"
#include "BackendMonitor.h"

int main()
{
  ofxMicromundos::setDataPathRoot();

  ofxJSON config = ofxMicromundos::load_config("config.json");
  ofxJSON backend_config = ofxMicromundos::load_config("backend.json");

  ofGLFWWindowSettings settings;

  //main app win
  shared_ptr<ofAppBaseWindow> app_win = ofCreateWindow(settings);

  //backend monitor win
  settings.width = backend_config["monitor"]["width"].asInt();
  settings.height = backend_config["monitor"]["height"].asInt();
  settings.windowMode = OF_WINDOW; 
  settings.setPosition(ofVec2f(backend_config["monitor"]["x"].asFloat(), backend_config["monitor"]["y"].asFloat()));
  settings.resizable = false;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> backend_monitor_win = ofCreateWindow(settings);

  //apps

  shared_ptr<ofApp> app(new ofApp);
  shared_ptr<BackendMonitor> backend_monitor_app(new BackendMonitor);
  shared_ptr<Backend> backend(new Backend);
  shared_ptr<GUI> backend_gui(new GUI);

  app->inject(config, backend_config, backend, backend_gui);
  backend_monitor_app->inject(backend, backend_gui);

  //run

  ofRunApp(app_win, app);
  ofRunApp(backend_monitor_win, backend_monitor_app);

  ofRunMainLoop();
}
