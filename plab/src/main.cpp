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

  ofGLFWWindowSettings settings;

  //app -> projector
  settings.width = 1024.;
  settings.height = 768.; 
  settings.setPosition(ofVec2f(0,0));
  settings.resizable = false;
  settings.decorated = false;
  shared_ptr<ofAppBaseWindow> app_win = ofCreateWindow(settings);

  //gui
  settings.width = 220;
  settings.height = 600;
  settings.setPosition(ofVec2f(0,0));
  settings.resizable = true;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> gui_win = ofCreateWindow(settings);

  //backend monitor
  settings.width = 640;
  settings.height = 480;
  settings.setPosition(ofVec2f(240,0));
  settings.resizable = false;
  settings.decorated = true;
  settings.shareContextWith = app_win;
  shared_ptr<ofAppBaseWindow> backend_monitor_win = ofCreateWindow(settings);

  shared_ptr<ofApp> app(new ofApp);
  shared_ptr<GUI> gui(new GUI);
  shared_ptr<BackendMonitorApp> backend_monitor(new BackendMonitorApp);

  app->inject(gui);
  backend_monitor->inject(app, gui);

  ofRunApp(gui_win, gui);
  ofRunApp(app_win, app);
  ofRunApp(backend_monitor_win, backend_monitor);

  ofRunMainLoop();
}
