#include "ofMain.h"
#include "ofApp.h"
#include "CalibGui.h"
#include "ofAppGLFWWindow.h"

int main()
{
  ofGLFWWindowSettings settings;

	settings.width = 1024;
	settings.height = 700;
	settings.setPosition(ofVec2f(240,0));
	settings.resizable = false;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

	settings.width = 220;
	settings.height = 700;
	settings.setPosition(ofVec2f(0,0));
	settings.resizable = false;
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);

	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<CalibGui> guiApp(new CalibGui);
	mainApp->gui = guiApp;

	ofRunApp(guiWindow, guiApp);
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
}
