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

      //if ( !pixels.isAllocated() )
        //pixels.allocate(dw, dh, 1);
      //int i = 0;
      //for (int x = 0; x < dw; x++)
      //for (int y = 0; y < dh; y++)
        //pixels[i++] = _plane.distance(rgbd->point(x,y));

      if ( !process.inited() )
      { 
        process
          .init( "glsl/height_map.frag", dw, dh, 3 )
          .on( "update", this, 
            &HeightMap::update_process );
      } 

      //TODO get a flipped point cloud if RGBD_Astra
      //vector<ofVec3f>& pcl = rgbd->point_cloud();
      //float* pcd = &(pcl.data())[0].x;
      float* pcd = new float[process.size()];
      int i = 0;
      for (int y = 0; y < dh; y++)
      for (int x = 0; x < dw; x++)
      {
        const ofVec3f& point = rgbd->point(x, y);
        pcd[i++] = point.x;
        pcd[i++] = point.y;
        pcd[i++] = point.z;
      }

      process
        .set( "point_cloud", pcd )
        .update(); 
      delete[] pcd;

      process.update_render();
      pixels = process
        .get_render()
        .get_data_pix(); //copy

      if (gui->log_height_map)
      {
        gui->log_height_map = false;
        process.log(dw-10, 10);
        //process.log(int(dw/2), int(dh/2));
      }

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
      process.off("update", this, &HeightMap::update_process);
      process.dispose();
    };

  private: 

    shared_ptr<CalibGui> gui;

    ofxPlane _plane;
    gpgpu::Process process;
    ofFloatPixels pixels;
    //ofFloatImage image;
    string filename;

    void update_process( ofShader& shader )
    {
      shader.setUniform4f( "plane", _plane.a, _plane.b, _plane.c, _plane.d );
    };


    //TODO HeightMap save/load from gui

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

