#pragma once

#include "ofxChilitags.h"
#include "ofxMicromundos/RGBD.h"
#include "ofxMicromundos/RGBD_Kinect2.h"
#include "ofxCv.h"
#include "TableCalib.h"
#include "HeightMap.h"
#include "ofxGeom.h"

class Calib
{
  public:

    Calib() {};
    ~Calib() 
    {
      dispose();
    };

    void init(shared_ptr<CalibGui>& gui)
    {
      rgbd = make_shared<ofxMicromundos::RGBD_Kinect2>();
      rgbd->init();

      chilitags.init(); 
      table_calib.init(gui);
      height_map.init(gui);

      proy_pts.push_back(ofVec2f(1/3,1/3));
      proy_pts.push_back(ofVec2f(2/3,1/3));
      proy_pts.push_back(ofVec2f(2/3,2/3));
      proy_pts.push_back(ofVec2f(1/3,2/3));

      contourFinder.setThreshold(128.); //15.
    };

    void update()
    {
      rgbd->update();

      vector<ofVec2f> tags_pts = get_tags_pts();
      vector<ofVec2f> depth_pts = get_depth_pts();  

      //TODO Calib find homography -> distortion coeffs ???

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
    };

    void render(float x, float y, float w, float h)
    {
      if (!rgbd->inited())
        return;

      float hw = w/2;
      float hh = h/2;

      //top left
      rgbd->render_grey_depth( x, y, hw, hh );
      table_calib.render( rgbd, x, y, hw, hh );

      //top right
      rgbd->render_rgb( x+hw, y, hw, hh );
      chilitags.render( x+hw, y, hw, hh );

      //bottom left
      height_map.render( x, y+hh, hw, hh );
    };

    void dispose()
    {};

  private:

    ofxChilitags chilitags;
    shared_ptr<ofxMicromundos::RGBD> rgbd;

    TableCalib table_calib;
    HeightMap height_map;
    //HomographyCalib H_rgb_proy;
    //HomographyCalib H_rgb_depth;
    //HomographyCalib H_depth_proy;

    vector<ofVec2f> proy_pts;
    ofxCv::ContourFinder contourFinder;


    vector<ofVec2f> get_tags_pts()
    {
      vector<ofVec2f> tags_pts;

      if (rgbd->rgb_updated())
      {
        ofPixels& rgb_pix = rgbd->rgb_pixels();

        chilitags.update(rgb_pix);
        vector<ChiliTag>& tags = chilitags.tags();
        if (!tags.empty())
          for (int i = 0; i < tags.size(); i++)
            tags_pts.push_back(tags[i].center_n);
      }

      return tags_pts;
    };

    vector<ofVec2f> get_depth_pts()
    {
      vector<ofVec2f> depth_pts;

      if (rgbd->depth_updated())
      {

        table_calib.update(rgbd);
        ofxPlane table_plane = table_calib.plane();
        height_map.plane(table_plane);

        ofFloatPixels height_map_pix = height_map
          .update(rgbd)
          .get();

        //TODO Calib get thresholded height map

        //contourFinder.findContours(height_map);

        //ofVec2f depth_size(rgbd->depth_width(), rgbd->depth_height());

        //for (int i = 0; i < contourFinder.size(); i++)
        //{
          //vector<cv::Point>& blob = contourFinder.getContour(i);
          //ofVec2f center;
          //for (int j = 0; j < blob.size(); j++)
            //center += ofxCv::toOf(blob[j]);
          //center /= blob.size();
          //center /= depth_size;
          //depth_pts.push_back(center);
        //}
      }

      return depth_pts;
    };

};

