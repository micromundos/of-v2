#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "ofApp.h"
#include "ofxMicromundos/GUI.h"
#include "ofxMicromundos/Backend.h"
#include "ofxMicromundos/utils.h"

int main()
{
  ofxMicromundos::setDataPathRoot();

  cv::FileStorage config = ofxMicromundos::load_config("config.yml");
  cv::FileStorage server_config = ofxMicromundos::load_config("server.yml");

  ofGLFWWindowSettings settings;

  //backend server
  settings.width = server_config["backend_server"]["width"];
  settings.height = server_config["backend_server"]["height"];
  settings.windowMode = OF_WINDOW; 
  settings.setPosition(ofVec2f(server_config["backend_server"]["x"], server_config["backend_server"]["y"]));
  settings.resizable = false;
  settings.decorated = true;
  shared_ptr<ofAppBaseWindow> app_win = ofCreateWindow(settings);

  shared_ptr<GUI> gui(new GUI);
  shared_ptr<ofApp> app(new ofApp);

  app->inject(gui, config, server_config);

  ofRunApp(app_win, app);

  ofRunMainLoop();
}
