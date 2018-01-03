#include "ofMain.h"
#include "ofApp.h"
#include "GUI.h"
#include "BackendMonitor.h"
#include "ofAppGLFWWindow.h"

int main()
{
  ofGLFWWindowSettings settings;

  //app -> projector
  settings.width = 400;//1024;
  settings.height = 400;//768; 
  settings.setPosition(ofVec2f(0,0));
  settings.resizable = false;
  settings.decorated = false;
  shared_ptr<ofAppBaseWindow> app_win = ofCreateWindow(settings);

  //gui
  settings.width = 220;
  settings.height = 600;
  settings.setPosition(ofVec2f(0,0));
  settings.resizable = false;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> gui_win = ofCreateWindow(settings);

  //backend monitor
  settings.width = 800;
  settings.height = 600;
  settings.setPosition(ofVec2f(240,0));
  settings.resizable = false;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> backend_monitor_win = ofCreateWindow(settings);

  shared_ptr<ofApp> app(new ofApp);
  shared_ptr<GUI> gui(new GUI);
  shared_ptr<BackendMonitor> backend_monitor(new BackendMonitor);

  app->gui = gui;
  backend_monitor->gui = gui;
  backend_monitor->app = app;

  ofRunApp(gui_win, gui);
  ofRunApp(app_win, app);
  ofRunApp(backend_monitor_win, backend_monitor);

  ofRunMainLoop();
}
