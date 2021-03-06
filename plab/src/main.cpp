#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"
#include "MonitorApp.h"
#include "plab/GUI.h"
#include "ofxMicromundos/utils.h"
#include "ofxMicromundos/Backend.h"
#include "ofxJSON.h"

int main()
{
  ofxMicromundos::setDataPathRoot();

  ofxJSON config = ofxMicromundos::load_config("config.json");
  ofxJSON plab_config = ofxMicromundos::load_config("plab.json");

  ofGLFWWindowSettings settings;

  //projector
  settings.width = config["projector"]["width"].asInt();
  settings.height = config["projector"]["height"].asInt();
  settings.windowMode = OF_WINDOW; 
  settings.setPosition(ofVec2f(plab_config["projector"]["x"].asFloat(), plab_config["projector"]["y"].asFloat()));
  settings.resizable = false;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> app_win = ofCreateWindow(settings);

  //monitor
  settings.width = plab_config["monitor"]["width"].asInt();
  settings.height = plab_config["monitor"]["height"].asInt();
  settings.windowMode = OF_WINDOW; 
  settings.setPosition(ofVec2f(plab_config["monitor"]["x"].asFloat(), plab_config["monitor"]["y"].asFloat()));
  settings.resizable = false;
  settings.decorated = true;
  settings.shareContextWith = app_win;
  shared_ptr<ofAppBaseWindow> monitor_win = ofCreateWindow(settings);

  shared_ptr<GUI> gui(new GUI);
  shared_ptr<ofApp> app(new ofApp);
  shared_ptr<MonitorApp> monitor(new MonitorApp);

  app->inject(gui, config, plab_config);
  monitor->inject(app, gui);

  ofRunApp(app_win, app);
  ofRunApp(monitor_win, monitor);

  ofRunMainLoop();
}
