#include "ofApp.h"

void ofApp::setup()
{
  ofxMicromundos::setDataPathRoot();

  ofSetLogLevel(OF_LOG_NOTICE);
  ofSetVerticalSync(true);

  ofSetWindowPosition(10,10);
  ofSetWindowShape(640*2,480);
  ofSetFullscreen(false);


  rgbd = make_shared<ofxMicromundos::RGBD_Astra>();
  rgbd->init();

  chilitags.init(); 
  table.init();

  proy_pts.push_back(ofVec2f(1/3,1/3));
  proy_pts.push_back(ofVec2f(2/3,1/3));
  proy_pts.push_back(ofVec2f(2/3,2/3));
  proy_pts.push_back(ofVec2f(1/3,2/3));

  contourFinder.setThreshold(128.); //15.
}


void ofApp::update()
{
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

  vector<ofVec2f> tags_pts;
  vector<ofVec2f> depth_pts; 

  rgbd->update();

  if (rgbd->rgb_updated())
  {
    ofPixels& rgb_pix = rgbd->get_rgb_pixels();

    chilitags.update(rgb_pix);
    vector<ChiliTag>& tags = chilitags.tags();
    if (!tags.empty())
      for (int i = 0; i < tags.size(); i++)
        tags_pts.push_back(tags[i].center_n);
  }

  if (rgbd->depth_updated())
  {
    ofFloatPixels depth_pix = rgbd->get_depth_pixels();
    ofVec2f depth_size(depth_pix.getWidth(), depth_pix.getHeight());

    ofFloatPixels height_map = table
      .update(rgbd)
      .get_height_map(); 

    contourFinder.findContours(height_map);

    for (int i = 0; i < contourFinder.size(); i++)
    {
      vector<cv::Point>& blob = contourFinder.getContour(i);
      ofVec2f center;
      for (int j = 0; j < blob.size(); j++)
        center += ofxCv::toOf(blob[j]);
      center /= blob.size();
      center /= depth_size;
      depth_pts.push_back(center);
    }
  }  


  if (!tags_pts.empty())
  {
    //H_rgb_proy.find(tags_pts, proy_pts);
    //H_rgb_proy.save();
  }

  if (!depth_pts.empty())
  {
    //H_depth_proy.find(depth_pts, proy_pts);
    //H_depth_proy.save();
  }

  if (!tags_pts.empty() && !depth_pts.empty())
  {
    //H_rgb_depth.find(tags_pts, depth_pts);
    //H_rgb_depth.save();
  } 
}


void ofApp::draw()
{
  float w = ofGetWidth()/2;
  float h = ofGetHeight();
  rgbd->render_rgb(0, 0, w, h);
  rgbd->render_grey_depth(w, 0, w, h);
  chilitags.render(0, 0, w, h);
}


void ofApp::keyPressed(int key)
{

}


void ofApp::keyReleased(int key)
{

}


void ofApp::mouseMoved(int x, int y )
{

}


void ofApp::mouseDragged(int x, int y, int button)
{

}


void ofApp::mousePressed(int x, int y, int button)
{

}


void ofApp::mouseReleased(int x, int y, int button)
{

}


void ofApp::mouseEntered(int x, int y)
{

}


void ofApp::mouseExited(int x, int y)
{

}


void ofApp::windowResized(int w, int h)
{

}


void ofApp::gotMessage(ofMessage msg)
{

}


void ofApp::dragEvent(ofDragInfo dragInfo)
{ 

}
