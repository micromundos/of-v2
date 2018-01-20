#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"
#include "BackendMonitorApp.h"
#include "GUI.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/utils.h"

int main()
{
  ofxMicromundos::setDataPathRoot();

  cv::FileStorage config = ofxMicromundos::load_config("config.yml");
  cv::FileStorage plab_config = ofxMicromundos::load_config("plab.yml");

  ofGLFWWindowSettings settings;

  //app -> projector
  settings.width = config["projector"]["width"];
  settings.height = config["projector"]["height"]; 
  int fullscreen = config["projector"]["fullscreen"];
  if (fullscreen != 0) settings.windowMode = OF_FULLSCREEN; 
  settings.setPosition(ofVec2f(0,0));
  settings.resizable = false;
  settings.decorated = false;
  shared_ptr<ofAppBaseWindow> app_win = ofCreateWindow(settings);

  //gui
  settings.width = config["gui"]["width"];
  settings.height = config["gui"]["height"];
  settings.setPosition(ofVec2f(config["gui"]["x"],config["gui"]["y"]));
  settings.resizable = true;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> gui_win = ofCreateWindow(settings);

  //backend monitor
  settings.width = plab_config["backend_monitor"]["width"];
  settings.height = plab_config["backend_monitor"]["height"];
  settings.setPosition(ofVec2f(plab_config["backend_monitor"]["x"], plab_config["backend_monitor"]["y"]));
  settings.resizable = false;
  settings.decorated = true;
  settings.shareContextWith = app_win;
  shared_ptr<ofAppBaseWindow> backend_monitor_win = ofCreateWindow(settings);

  shared_ptr<ofApp> app(new ofApp);
  shared_ptr<GUI> gui(new GUI);
  shared_ptr<BackendMonitorApp> backend_monitor(new BackendMonitorApp);

  app->inject(gui, config, plab_config);
  backend_monitor->inject(app, gui);

  ofRunApp(gui_win, gui);
  ofRunApp(app_win, app);
  ofRunApp(backend_monitor_win, backend_monitor);

  ofRunMainLoop();
}
