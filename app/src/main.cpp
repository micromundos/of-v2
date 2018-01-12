#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"
#include "BackendMonitorApp.h"
#include "GUI.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/data_path.h"

int main()
{
  ofxMicromundos::setDataPathRoot();

  cv::FileStorage config( ofToDataPath("config.yml", false), cv::FileStorage::READ );
  if (!config.isOpened())
  {
    ofLogError() << "failed to load config.yml";
    ofExit();
  }

  ofGLFWWindowSettings settings;

  //app -> projector
  settings.width = config["projector_width"];
  settings.height = config["projector_height"]; 
  settings.setPosition(ofVec2f(0,0));
  settings.resizable = false;
  settings.decorated = false;
  shared_ptr<ofAppBaseWindow> app_win = ofCreateWindow(settings);

  //gui
  settings.width = config["gui_width"];
  settings.height = config["gui_height"];
  settings.setPosition(ofVec2f(config["gui_x"],config["gui_y"]));
  settings.resizable = true;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> gui_win = ofCreateWindow(settings);

  //backend monitor
  settings.width = config["backend_monitor_width"];
  settings.height = config["backend_monitor_height"];
  settings.setPosition(ofVec2f(config["backend_monitor_x"],config["backend_monitor_y"]));
  settings.resizable = false;
  settings.decorated = true;
  settings.shareContextWith = app_win;
  shared_ptr<ofAppBaseWindow> backend_monitor_win = ofCreateWindow(settings);

  shared_ptr<ofApp> app(new ofApp);
  shared_ptr<GUI> gui(new GUI);
  shared_ptr<BackendMonitorApp> backend_monitor(new BackendMonitorApp);

  app->inject(gui, config);
  backend_monitor->inject(app, gui);

  ofRunApp(gui_win, gui);
  ofRunApp(app_win, app);
  ofRunApp(backend_monitor_win, backend_monitor);

  ofRunMainLoop();
}
