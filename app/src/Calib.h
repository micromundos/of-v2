#pragma once

#include "CalibGui.h"
#include "ofxChilitags.h"

using namespace ofxCv;

class Calib
{
  public:

    Calib() {};
    ~Calib() 
    {
      dispose();
    };

    bool init()
    {
      file_calib = "calib/H_rgb_proj.yml";
      proj_pts.push_back(cv::Point2f(1./3.,1./3.));
      proj_pts.push_back(cv::Point2f(2./3.,1./3.));
      proj_pts.push_back(cv::Point2f(2./3.,2./3.));
      proj_pts.push_back(cv::Point2f(1./3.,2./3.));
      if (load())
        return test();
    };

    bool update(vector<ChiliTag>& tags)
    {
      if (tags.empty())
        return test();

      vector<cv::Point2f> tags_pts;
      for (int i = 0; i < tags.size(); i++)
        tags_pts.push_back(tags[i].center_n_cv);

      H_cv = findHomography(
          cv::Mat(tags_pts), 
          cv::Mat(proj_pts));

      return test() && save();
    };

    void render(vector<ChiliTag>& tags, float x, float y, float w, float h)
    {
      ofVec2f scale(w,h);
      ofVec2f translate(x,y);

      ofPushStyle();
      ofSetColor(ofColor::yellow);

      for (int i = 0; i < tags.size(); i++)
      {
        vector<ofVec2f> &corners_n = tags[i].corners_n;
        ofVec2f pof0, pof1;
        cv::Point2f pcv0, pcv1;
        for (int j = 0; j < corners_n.size(); j++)
        {
          pof0 = corners_n[j] * scale + translate;
          pof1 = corners_n[ (j+1)%4 ] * scale + translate;

          //TODO calib: render test points with H applied
          //pcv0 = toCv(pof0) * H_cv;
          //pcv1 = toCv(pof1) * H_cv;

          ofDrawLine( pcv0.x, pcv0.y, pcv1.x, pcv1.y );
        }
      }

      ofPopStyle();
    };

    void dispose()
    {};

    float* homography()
    {
      return H_of.getPtr();
    };

  private:

    cv::Mat H_cv;
    ofMatrix4x4 H_of; 

    vector<cv::Point2f> proj_pts;
    string file_calib;

    //TODO calib: test points applying H
    bool test()
    {
      //vector<cv::Point2f> test_pts = tags_pts * H_cv;
      //return proj_pts - test_pts < threshold;
      return true;
    };

    bool save()
    {
      cv::FileStorage fs( ofToDataPath(file_calib, false), cv::FileStorage::WRITE );
      fs << "homography" << H_cv;
      return true;
    };

    bool load()
    {
      cv::FileStorage fs( ofToDataPath(file_calib, false), cv::FileStorage::READ ); 
      if ( !fs.isOpened() )
        return false;
      fs["homography"] >> H_cv;
      return true;
    };
};

