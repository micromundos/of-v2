#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"
#include "backend/Backend.h"
#include "BackendMonitorApp.h"
#include "GUI.h"

int main()
{
  ofGLFWWindowSettings settings;

  //app -> projector
  settings.width = 1024./2;
  settings.height = 768./2; 
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
  shared_ptr<BackendMonitorApp> backend_monitor(new BackendMonitorApp);

  shared_ptr<Backend> backend = make_shared<Backend>();

  app->inject(backend, gui);
  backend_monitor->inject(backend, gui);

  ofRunApp(gui_win, gui);
  ofRunApp(app_win, app);
  ofRunApp(backend_monitor_win, backend_monitor);

  ofRunMainLoop();
}
