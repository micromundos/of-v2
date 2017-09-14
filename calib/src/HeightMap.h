#pragma once

#include "ofMain.h"
#include "ofxMicromundos/RGBD.h"
#include "ofxCv.h"
#include "ofxGPGPU.h"
#include "ofxGeom.h"

class HeightMap
{
  public:

    HeightMap() {};
    ~HeightMap() 
    {
      dispose();
    };

    void init(shared_ptr<CalibGui>& gui)
    {
      this->gui = gui;
      filename = "calib/height_map.yml";
    }; 

    HeightMap& update(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      int dw = rgbd->depth_width();
      int dh = rgbd->depth_height();

      if ( !process.inited() )
      { 
        process
          .init( "glsl/height_map.frag", dw, dh, 3 )
          .on( "update", this, 
            &HeightMap::update_process );
        process
          .get_render()
          .on( "update", this, 
            &HeightMap::update_render_process );
      } 

      float* pcd = rgbd->point_cloud_data();

      process
        .set( "point_cloud", pcd )
        .update(); 

      process.update_render();
      pixels = process
        .get_render()
        .get_data_pix(); //copy

      if (gui->height_map_log)
      {
        gui->height_map_log = false;
        process.log(dw-10, 10);
        //process.log(int(dw/2), int(dh/2));
      }

      max_height = gui->height_map_threshold;

      return *this;
    };

    ofFloatPixels& get()
    {
      return pixels;
    };

    void plane(ofxPlane plane)
    {
      _plane = plane;
    };

    void render(float x, float y, float w, float h)
    { 
      if (!process.inited())
        return;
      process.render(x, y, w, h);
      //image.setFromPixels(pixels);
      //image.draw(x, y, w, h);
      ofDrawBitmapStringHighlight("height map", x, y+12);
    };

    void dispose()
    {
      process
        .off("update", this, &HeightMap::update_process);
      process
        .get_render()
        .off("update", this, &HeightMap::update_render_process);
      process.dispose();
    };

  private: 

    shared_ptr<CalibGui> gui;

    ofxPlane _plane;
    gpgpu::Process process;
    ofFloatPixels pixels;
    //ofFloatImage image;
    string filename;
    float max_height;

    void update_process( ofShader& shader )
    {
      shader.setUniform4f( "plane", _plane.a, _plane.b, _plane.c, _plane.d );
    };

    void update_render_process( ofShader& shader )
    {
      shader.setUniform1f( "max_height", max_height );
    };


    //TODO save/load from gui

    void save()
    {
      ofLog() << "\n";
      ofLogNotice("HeightMap") 
        << "save height map to " 
        << filename;

      //cv::Mat height_map_mat = ofxCv::toCv(image);
      cv::Mat height_map_mat = ofxCv::toCv(pixels);

      cv::FileStorage fs( ofToDataPath(filename, false), cv::FileStorage::WRITE );
      fs << "height_map" << height_map_mat;
    };

    void load()
    {
      ofLog() << "\n";
      ofLogNotice("HeightMap") 
        << "load height map from " 
        << filename;

      cv::FileStorage fs( ofToDataPath(filename, false), cv::FileStorage::READ );  

      if ( !fs.isOpened() )
      {
        ofLogError("HeightMap") << "failed to load table image file " << filename;
        return false;
      }

      cv::Mat height_map_mat;
      fs["height_map"] >> height_map_mat;
      ofFloatPixels pix;
      ofxCv::toOf( height_map_mat, pix );
      pixels = pix; //copy
      //image.setFromPixels( pix );
    };
};

