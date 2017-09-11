#pragma once

#include "ofMain.h"
#include "ofxMicromundos/RGBD.h"
#include "ofxCv.h"
#include "ofxGeom.h"
#include "CalibGui.h"

class TableCalib
{
  public:

    TableCalib() {};
    ~TableCalib() {};

    void init(shared_ptr<CalibGui>& gui)
    {
      this->gui = gui;
      file_bg = "calib/table_bg_depth_map.yml";
      file_plane = "calib/table_plane.yml";
    }; 

    void update(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      learn_bg(rgbd);
      if (find_table_plane(rgbd)) return;
      if (save()) return;
      if (load()) return;
    };

    ofxPlane plane() 
    {
      return _plane;
    };

    void render(const shared_ptr<ofxMicromundos::RGBD> &rgbd, float x, float y, float w, float h)
    {
      //bg_depth.draw( x, y, w, h );

      float dw = rgbd->depth_width();
      float dh = rgbd->depth_height();
      ofVec2f norm( dw, dh );
      ofVec2f scale( w, h );
      ofVec2f translate( x, y );
      render_planes_list(norm, scale, translate); 
      //render_plane(norm, scale, translate); 

      ofDrawBitmapStringHighlight("TableCalib learned background depth map", x, y+12);
    }; 

  private:

    shared_ptr<CalibGui> gui;

    string file_plane;
    string file_bg;

    ofFloatImage bg_depth;
    ofxPlane _plane;
    ofxTriangle _triangle; 
    vector<ofxTriangle> tris3d;

    //render / debug
    vector<ofxTriangle> tris2d;
    ofVboMesh tris2d_mesh;
    //ofxTriangle triangle_2d; 
 

    // calibration


    bool find_table_plane(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      if ( !gui->find_table_plane )
          //|| rgbd->depth_updated() )
        return false;
      gui->find_table_plane = !gui->find_table_plane; //once

      find_triangles(rgbd);
      _triangle = calc_avg_tri();
      _plane = _triangle.plane();
      //_plane = calc_avg_plane();

      return true;
    };

    void find_triangles(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      int dw = rgbd->depth_width();
      int dh = rgbd->depth_height();

      float r = gui->radius_step;
      float ang = gui->angle_step;

      ofVec2f ctr((float)dw/2,(float)dh/2);

      tris3d.resize( gui->planes_num );
      tris2d.resize( gui->planes_num );

      for ( int i = 0; i < gui->planes_num; i++ )
      {
        ofVec3f tri3d[3];
        ofVec3f tri2d[3];

        for (int j = 0; j < 3; j++)
        {
          tri2d[j] = ctr + ofVec2f(0,r).rotate( (360./3.)*j + ang ); 
          float dx = tri2d[j].x; 
          float dy = tri2d[j].y; 
          //TODO TableCalib extract point from learned bg point cloud to find triangles
          tri3d[j].set( rgbd->point(dx, dy) );
        } 

        tris3d[i] = ofxTriangle( tri3d[0], tri3d[1], tri3d[2] );
        tris2d[i] = ofxTriangle( tri2d[0], tri2d[1], tri2d[2] ); // z = 0

        r += gui->radius_step;
        ang += gui->angle_step;
      }
    };

    ofxTriangle calc_avg_tri()
    {
      int nplanes = tris3d.size();
      ofVec3f tri3d[3];

      for ( int i = 0; i < nplanes; i++ )
        for ( int j = 0; j < 3; j++ )
          tri3d[j] += tris3d[i].vertex(j);
      for ( int j = 0; j < 3; j++ )
        tri3d[j] /= nplanes;

      ofxTriangle t( tri3d[0], tri3d[1], tri3d[2] );
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

    //TODO TableCalib learn bg point cloud
    bool learn_bg(const shared_ptr<ofxMicromundos::RGBD> &rgbd)
    {
      if ( !gui->learn_bg 
          || !rgbd->depth_updated() )
        return false;

      ofFloatPixels depth_pix = rgbd->depth_pixels();

      if ( !bg_depth.isAllocated() ) 
        bg_depth.setFromPixels( depth_pix );

      int n = depth_pix.size();
      float* dpix = depth_pix.getData();
      float* bgpix = bg_depth.getPixels().getData();

      for ( int i = 0; i < n; i++ ) 
        if ( dpix[i] != 0 )
          bgpix[i] = (bgpix[i] + dpix[i]) * 0.5;

      bg_depth.update();

      return true;
    };


    // save & load


    bool initial_load()
    {
      ofLog() << "\n";
      ofLogNotice("TableCalib") << "initial load";
      return load_bg() && load_plane();
    }; 

    bool load()
    {
      if (!gui->load)
        return false;
      gui->load = false; //once
      return load_bg() && load_plane();
    }; 

    bool save()
    {
      if ( !gui->save )
        return false;
      gui->save = false; //once
      save_bg();
      save_plane(); 
      return true;
    };

    void save_bg()
    {
      ofLog() << "\n";
      ofLogNotice("TableCalib") 
        << "save bg_depth to " 
        << file_bg;

      cv::Mat image_mat = ofxCv::toCv( bg_depth );

      cv::FileStorage fs( ofToDataPath(file_bg, false), cv::FileStorage::WRITE );
      fs << "table_bg_depth_map" << image_mat; 
    };

    void save_plane()
    {
      ofxTriangle& tri = _triangle;
      ofVec3f v0 = tri.b;
      ofVec3f v1 = tri.b + tri.e0;
      ofVec3f v2 = tri.b + tri.e1;

      ofLog() << "\n";
      ofLogNotice("TableCalib") 
        << "save plane calib to " 
        << file_plane << "\n"
        << "v0 " << v0 << "\n"
        << "v1 " << v1 << "\n"
        << "v2 " << v2 << "\n";

      cv::FileStorage fs( ofToDataPath(file_plane, false), cv::FileStorage::WRITE ); 

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

    bool load_plane()
    {
      cv::FileStorage fs( ofToDataPath(file_plane, false), cv::FileStorage::READ ); 

      if ( !fs.isOpened() )
      {
        ofLogError("TableCalib") 
          << "failed to load plane calib file " 
          << file_plane;
        return false;
      }

      ofVec3f v0 = ofVec3f( fs["vertex_0_x"], fs["vertex_0_y"], fs["vertex_0_z"] );
      ofVec3f v1 = ofVec3f( fs["vertex_1_x"], fs["vertex_1_y"], fs["vertex_1_z"] );
      ofVec3f v2 = ofVec3f( fs["vertex_2_x"], fs["vertex_2_y"], fs["vertex_2_z"] );

      ofLog() << "\n";
      ofLogNotice("TableCalib")
        << "load plane calib from " 
        << file_plane << "\n"
        << "v0 " << v0 << "\n"
        << "v1 " << v1 << "\n"
        << "v2 " << v2 << "\n";

      _triangle = ofxTriangle(v0,v1,v2);
      _plane = _triangle.plane();

      return true;
    };

    bool load_bg()
    {
      ofLog() << "\n";
      ofLogNotice("TableCalib") 
        << "load bg_depth from " 
        << file_bg;

      cv::FileStorage fs( ofToDataPath(file_bg, false), cv::FileStorage::READ );  

      if ( !fs.isOpened() )
      {
        ofLogError("TableCalib") 
          << "failed to load table bg_depth file " 
          << file_bg;
        return false;
      }

      cv::Mat image_mat;
      fs["table_bg_depth_map"] >> image_mat;
      ofFloatPixels pix;
      ofxCv::toOf( image_mat, pix );
      bg_depth.setFromPixels( pix ); 

      return true;
    }; 

    void render_planes_list(const ofVec2f& norm, const ofVec2f& scale, const ofVec2f& translate)
    {
      tris2d_mesh.setMode(OF_PRIMITIVE_TRIANGLES);
      tris2d_mesh.clear();

      int nplanes = tris2d.size();
      ofFloatColor color;
      ofVec2f p2, p2_render;

      for ( int i = 0; i < nplanes; i++ )
      {
        float hue = ((float)(i+1)/nplanes);
        color = ofFloatColor::fromHsb(hue,1.,1.);
        for ( int j = 0; j < 3; j++ )
        {
          p2 = ofVec2f( tris2d[i].vertex(j) );
          p2_render = (p2 / norm) * scale + translate;
          tris2d_mesh.addColor( color );
          tris2d_mesh.addVertex( p2_render );
        }
      }
      tris2d_mesh.drawWireframe();
    };

    //void render_plane(const ofVec2f& norm, const ofVec2f& scale, const ofVec2f& translate)
    //{
      //ofPushStyle();
      //ofSetColor( ofColor::magenta );
      //ofVec2f tri2d[3];
      //for ( int i = 0; i < 3; i++ )
      //{
        //ofVec2f p2 = ofVec2f(triangle_2d.vertex(i));
        //tri2d[i] = (p2 / norm) * scale + translate;
      //}
      //for ( int i = 0; i < 3; i++ )
        //ofDrawLine( tri2d[i], tri2d[(i+1)%3] );
      //ofPopStyle();
    //};
};

