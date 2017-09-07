#pragma once

#include "ofMain.h"
#include "ofxMicromundos/RGBD.h"
#include "ofxCv.h"
#include "ofxGPGPU.h"
#include "ofxGeom.h"

class TableCalib
{
  public:

    TableCalib() {};
    ~TableCalib() 
    {
      dispose();
    };

    void init()
    {
      //TODO UI settings runtime
      filename_background_depth_map = "calib/table_background_depth_map.yml";
      filename_background_height_map = "calib/table_background_height_map.yml";
      filename_plane = "calib/table_plane.yml";
      _angle_step = 30; // [ 30, 0, 360 ]
      _planes_num = 20; // [ 20, 0, 100 ]
      _radius_step = 6; // [ 6, 0, 30 ]
      _calibrate = false;
      _learn_bg = false;
      _load = false;
      _save = false;
      _render_plane = false;
      _render_planes_list = false;
    };

    void dispose()
    {
      height_map.off( "update", this, &TableCalib::update_height_map_glsl );
    };

    TableCalib& update(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      learn_background_depth_map(rgbd);
      if (calibrate(rgbd))
        return *this;
      if (save())
        return *this;
      if (load())
        return *this;
    };

    ofFloatPixels& get_height_map()
    {
      return background_height_map.getPixels();
    };

  private:

    //TODO UI settings runtime
    string filename_background_depth_map;
    string filename_background_height_map;
    string filename_plane;
    ofParameter<bool> _calibrate;
    ofParameter<bool> _learn_bg;
    ofParameter<bool> _load;
    ofParameter<bool> _save;
    ofParameter<float> _planes_num;
    ofParameter<float> _radius_step;
    ofParameter<float> _angle_step;
    ofParameter<bool> _render_plane;
    ofParameter<bool> _render_planes_list;

    gpgpu::Process height_map;

    ofFloatImage background_depth_map;
    ofFloatImage background_height_map;

    ofxTriangle triangle; 
    ofxPlane plane;
    vector<ofxTriangle> tris3d;

    //debugging
    vector<ofxTriangle> tris2d;
    ofVboMesh tris2d_mesh;
    ofVboMesh tris3d_mesh;


    //glsl


    void update_height_map(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      if ( !rgbd->depth_updated() )
        return;

      if ( !height_map.inited() )
      {
        int w = rgbd->get_depth_pixels().getWidth();
        int h = rgbd->get_depth_pixels().getHeight();
        height_map
          .init( "glsl/height_map.frag", w, h )
          .on( "update", this, 
            &TableCalib::update_height_map_glsl );
      }

      vector<ofVec3f>& point_cloud = rgbd->get_point_cloud();
      ofVec3f* pts = point_cloud.data();
      float* point_cloud_data = &(pts)[0].x;

      height_map
        .set( "mesh3d", point_cloud_data )
        .update(); 

      background_height_map.setFromPixels( height_map.get_data_pix() );
    };

    void update_height_map_glsl( ofShader& shader )
    {
      shader.setUniform4f( "plane", plane.a, plane.b, plane.c, plane.d );
    };
 

    // calibration


    bool calibrate(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      if ( !_calibrate || !rgbd->depth_updated() )
        return false;
      _calibrate = !_calibrate; //once

      find_triangles(rgbd);
      triangle = calc_avg_tri();
      plane = triangle.plane();
      //plane = calc_avg_plane();

      //done on learn_background_depth_map
      //background_depth_map.setFromPixels(rgbd->get_depth_pixels());

      update_background_height_map(rgbd);

      return true;
    };

    void find_triangles(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      int w = rgbd->get_depth_pixels().getWidth();
      int h = rgbd->get_depth_pixels().getHeight();

      float r = _radius_step;
      float ang = _angle_step;

      ofVec2f ctr((float)w/2,(float)h/2);

      tris3d.resize( _planes_num );
      tris2d.resize( _planes_num );

      for ( int i = 0; i < _planes_num; i++ )
      {
        ofVec3f tri3d[3];
        ofVec3f tri2d[3];

        for (int j = 0; j < 3; j++)
        {
          tri2d[j] = ctr + ofVec2f(0,r).rotate( (360./3.)*j + ang ); 
          float dx = tri2d[j].x; 
          float dy = tri2d[j].y; 
          tri3d[j].set( rgbd->get_point( dx, dy ) );
        } 

        tris3d[i] = ofxTriangle( tri3d[0], tri3d[1], tri3d[2] );
        tris2d[i] = ofxTriangle( tri2d[0], tri2d[1], tri2d[2] );

        r += _radius_step;
        ang += _angle_step;
      }
    };

    ofxTriangle calc_avg_tri()
    {
      int nplanes = tris3d.size();
      ofVec3f tri[3];

      for ( int i = 0; i < nplanes; i++ )
        for ( int j = 0; j < 3; j++ )
          tri[j] += tris3d[i].vertex(j);
      for ( int j = 0; j < 3; j++ )
        tri[j] /= nplanes;

      ofxTriangle t(tri[0],tri[1],tri[2]);
      return t;
    };

    ofxPlane calc_avg_plane()
    {
      int nplanes = tris3d.size();
      ofVec3f normal;
      ofVec3f ctr;

      for ( int i = 0; i < nplanes; i++ )
      {
        normal += tris3d[i].normal();
        ctr += tris3d[i].centroid();
      }
      normal /= nplanes;
      ctr /= nplanes;

      return ofxPlane( ctr, normal );
    };

    bool update_background_height_map(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      if ( !background_depth_map.isAllocated() || rgbd->depth_updated() )
        return false;

      update_height_map(rgbd);

      return true;
    }; 

    bool learn_background_depth_map(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      if ( !_learn_bg || !rgbd->depth_updated() )
        return false;

      ofFloatPixels depth_pix = rgbd->get_depth_pixels();

      if ( !background_depth_map.isAllocated() )
        background_depth_map.setFromPixels( depth_pix );

      int n = depth_pix.size();
      float* dpix = depth_pix.getData();
      float* bgpix = background_depth_map.getPixels().getData();

      for ( int i = 0; i < n; i++ ) 
        if ( dpix[i] != 0 )
          bgpix[i] = (bgpix[i] + dpix[i]) * 0.5;

      background_depth_map.update();

      return true;
    };


    // save & load


    bool initial_load()
    {
      ofLogNotice("TableCalib") << "initial load";
      return load_background() && load_table_plane();
    }; 

    bool load()
    {
      if (!_load)
        return false;
      _load = false; //once
      return load_background() && load_table_plane();
    }; 

    bool save()
    {
      if ( !_save )
        return false;
      _save = false; //once
      save_background();
      save_table_plane(); 
      return true;
    };

    void save_background()
    {
      // save background depth map

      ofLogNotice("TableCalib") 
        << "save background_depth_map to " 
        << filename_background_depth_map;

      cv::Mat _background_depth_map = ofxCv::toCv( background_depth_map );

      cv::FileStorage fs_bg_depth( ofToDataPath(filename_background_depth_map, false), cv::FileStorage::WRITE );
      fs_bg_depth << "table_background_depth_map" << _background_depth_map;

      // save background height map

      ofLogNotice("TableCalib") 
        << "save background_height_map to " 
        << filename_background_height_map;

      cv::Mat _background_height_map = ofxCv::toCv( background_height_map );

      cv::FileStorage fs_bg_height( ofToDataPath(filename_background_height_map, false), cv::FileStorage::WRITE );
      fs_bg_height << "table_background_height_map" << _background_height_map;
    };

    void save_table_plane()
    {
      ofxTriangle& tri = triangle;
      ofVec3f v0 = tri.b;
      ofVec3f v1 = tri.b + tri.e0;
      ofVec3f v2 = tri.b + tri.e1;

      ofLogNotice("TableCalib") 
        << "save plane calib to " 
        << filename_plane << "\n"
        << "v0 " << v0 << "\n"
        << "v1 " << v1 << "\n"
        << "v2 " << v2 << "\n";

      cv::FileStorage fs( ofToDataPath(filename_plane, false), cv::FileStorage::WRITE ); 

      fs << "vertex_0_x" << v0.x;
      fs << "vertex_0_y" << v0.y;
      fs << "vertex_0_z" << v0.z;

      fs << "vertex_1_x" << v1.x;
      fs << "vertex_1_y" << v1.y;
      fs << "vertex_1_z" << v1.z;

      fs << "vertex_2_x" << v2.x;
      fs << "vertex_2_y" << v2.y;
      fs << "vertex_2_z" << v2.z;
    }; 


    bool load_table_plane()
    {
      cv::FileStorage fs( ofToDataPath(filename_plane, false), cv::FileStorage::READ ); 

      if ( !fs.isOpened() )
      {
        ofLogError("TableCalib") 
          << "failed to load plane calib file " 
          << filename_plane;
        return false;
      }

      ofVec3f v0 = ofVec3f( fs["vertex_0_x"], fs["vertex_0_y"], fs["vertex_0_z"] );
      ofVec3f v1 = ofVec3f( fs["vertex_1_x"], fs["vertex_1_y"], fs["vertex_1_z"] );
      ofVec3f v2 = ofVec3f( fs["vertex_2_x"], fs["vertex_2_y"], fs["vertex_2_z"] );

      ofLogNotice("TableCalib")
        << "load plane calib from " 
        << filename_plane << "\n"
        << "v0 " << v0 << "\n"
        << "v1 " << v1 << "\n"
        << "v2 " << v2 << "\n";

      triangle = ofxTriangle(v0,v1,v2);
      plane = triangle.plane();

      return true;
    };

    bool load_background()
    {
      // load background depth map

      ofLogNotice("TableCalib") 
        << "load background_depth_map from " 
        << filename_background_depth_map;

      cv::FileStorage fs_bg_depth( ofToDataPath(filename_background_depth_map, false), cv::FileStorage::READ );  

      if ( !fs_bg_depth.isOpened() )
      {
        ofLogError("TableCalib") 
          << "failed to load table background_depth_map file " 
          << filename_background_depth_map;
        return false;
      }

      cv::Mat _background_depth_map;
      fs_bg_depth["table_background_depth_map"] >> _background_depth_map;
      ofFloatPixels bg_depth;
      ofxCv::toOf( _background_depth_map, bg_depth );
      background_depth_map.setFromPixels( bg_depth );

      // load background height map

      ofLogNotice("TableCalib") 
        << "load background_height_map from " 
        << filename_background_height_map;

      cv::FileStorage fs_bg_height( ofToDataPath(filename_background_height_map, false), cv::FileStorage::READ );  

      if ( !fs_bg_height.isOpened() )
      {
        ofLogError("TableCalib") << "failed to load table background_height_map file " << filename_background_height_map;
        return false;
      }

      cv::Mat _background_height_map;
      fs_bg_height["table_background_height_map"] >> _background_height_map;
      ofFloatPixels bg_height;
      ofxCv::toOf( _background_height_map, bg_height );
      background_height_map.setFromPixels( bg_height );

      return true;
    }; 
};

