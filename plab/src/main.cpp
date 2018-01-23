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
  cv::FileStorage server_config = ofxMicromundos::load_config("server.yml");
  cv::FileStorage plab_config = ofxMicromundos::load_config("plab.yml");

  ofGLFWWindowSettings settings;

  //app -> projector
  settings.width = config["projector"]["width"];
  settings.height = config["projector"]["height"]; 
  if (string(config["projector"]["fullscreen"]).compare("true") == 0) 
    settings.windowMode = OF_FULLSCREEN; 
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
  settings.width = plab_config["monitor"]["width"];
  settings.height = plab_config["monitor"]["height"];
  settings.setPosition(ofVec2f(plab_config["monitor"]["x"], plab_config["monitor"]["y"]));
  settings.resizable = false;
  settings.decorated = true;
  settings.shareContextWith = app_win;
  shared_ptr<ofAppBaseWindow> monitor_win = ofCreateWindow(settings);

  shared_ptr<ofApp> app(new ofApp);
  shared_ptr<GUI> gui(new GUI);
  shared_ptr<BackendMonitorApp> monitor(new BackendMonitorApp);

  app->inject(gui, config, server_config, plab_config);
  monitor->inject(app, gui);

  ofRunApp(gui_win, gui);
  ofRunApp(app_win, app);
  ofRunApp(monitor_win, monitor);

  ofRunMainLoop();
}
