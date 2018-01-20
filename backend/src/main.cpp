#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"
#include "CalibProjApp.h"
#include "GUI.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/data_path.h"
#include "ofxMicromundos/load_config.h"

int main()
{
  ofxMicromundos::setDataPathRoot();

  cv::FileStorage config = ofxMicromundos::load_config("config.yml");
  cv::FileStorage server_config = ofxMicromundos::load_config("server.yml");

  ofGLFWWindowSettings settings;

  //backend server
  settings.width = server_config["backend_server"]["width"];
  settings.height = server_config["backend_server"]["height"];
  settings.setPosition(ofVec2f(server_config["backend_server"]["x"], server_config["backend_server"]["y"]));
  settings.resizable = false;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> app_win = ofCreateWindow(settings);

  //gui
  settings.width = config["gui"]["width"];
  settings.height = config["gui"]["height"];
  settings.setPosition(ofVec2f(config["gui"]["x"],config["gui"]["y"]));
  settings.resizable = true;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> gui_win = ofCreateWindow(settings);

  //calib -> projector
  settings.width = config["projector"]["width"];
  settings.height = config["projector"]["height"]; 
  int fullscreen = config["projector"]["fullscreen"];
  if (fullscreen != 0) settings.windowMode = OF_FULLSCREEN; 
  settings.setPosition(ofVec2f(0,0));
  settings.resizable = false;
  settings.decorated = false;
  settings.shareContextWith = app_win;
  shared_ptr<ofAppBaseWindow> calib_win = ofCreateWindow(settings);

  shared_ptr<ofApp> app(new ofApp);
  shared_ptr<GUI> gui(new GUI);
  shared_ptr<CalibProjApp> calib(new CalibProjApp);

  app->inject(gui, config, server_config);
  calib->inject(app, config);

  ofRunApp(gui_win, gui);
  ofRunApp(app_win, app);
  ofRunApp(calib_win, calib);

  ofRunMainLoop();
}