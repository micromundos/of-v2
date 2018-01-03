#pragma once

#include "GUI.h"
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
      H_ready = false;
      file_calib = "calib/H_rgb_proj.yml";

      float w = ofGetWidth();
      float h = ofGetHeight();

      proj_pts.push_back(cv::Point2f(w*1./3.,h*1./3.));
      proj_pts.push_back(cv::Point2f(w*2./3.,h*1./3.));
      proj_pts.push_back(cv::Point2f(w*2./3.,h*2./3.));
      proj_pts.push_back(cv::Point2f(w*1./3.,h*2./3.));

      coords.push_back(ofVec2f(-1.,-1.));
      coords.push_back(ofVec2f(1.,-1.));
      coords.push_back(ofVec2f(1.,1.));
      coords.push_back(ofVec2f(-1.,1.));

      //TODO load calib homography
      //load();
      return isReady();
    };

    bool update(vector<ChiliTag>& tags)
    {
      if (tags.size() != proj_pts.size())
        return isReady();

      float pw = ofGetWidth();
      float ph = ofGetHeight(); 

      ofVec2f ctr = tags_ctr(tags);
      tags_pts.clear(); 
      for (int i = 0; i < tags.size(); i++)
      { 
        //ofVec2f t = tags[i].center_n;
        ofVec2f t = tag_coord(coords[i], ctr, tags);
        t.x *= pw;
        t.y *= ph;
        tags_pts.push_back(toCv(t));
      }

      H_cv = cv::findHomography(
          cv::Mat(tags_pts), 
          cv::Mat(proj_pts));
      H_ready = true;

      H_cv2of(H_cv, H_of);

      return isReady() && save();
    }; 

    void render(float x, float y, float w, float h)
    {
      // 4 crosses:
      // + +
      // + +

      //float pw = ofGetWidth();
      //float ph = ofGetHeight();
      //ofVec2f pscale(pw,ph); 
      //ofVec2f scale(w,h);
      //ofVec2f translate(x,y);

      float s = 10;
      for (int i = 0; i < proj_pts.size(); i++)
      {
        ofVec2f p = toOf(proj_pts[i]);///pscale*scale + translate;
        ofDrawLine( p.x, p.y-s, p.x, p.y+s );
        ofDrawLine( p.x-s, p.y, p.x+s, p.y );
      }
    };

    void debug(ofPixels& pix, vector<ChiliTag>& tags, float x, float y, float w, float h)
    {
      debug_pixels(pix, x, y, w, h);
      debug_tags(tags, x, y, w, h);
    };

    void dispose()
    {
      debug_pix.clear();
      debug_tex.clear();
    }; 

    void transform(const ofVec2f &src, ofVec2f &dst)
    {
      dst.set(H_of.preMult(ofVec3f(src.x, src.y, 0)));
      //TODO calib transform points with cv::perspectiveTransform
      //cv::Mat srcMat = toCv(src), dstMat = toCv(dst);
      //cv::Mat transform = cv::getPerspectiveTransform(&srcPoints[0], &dstPoints[0]);
      //cv::perspectiveTransform(InputArray src, OutputArray dst, transform);
    };

    void transform(ofPixels& src, ofPixels& dst)
    {
      cv::Mat srcMat = toCv(src);
      cv::Mat dstMat = toCv(dst);
      cv::warpPerspective(srcMat, dstMat, H_cv, srcMat.size(), cv::INTER_LINEAR);
      toOf(dstMat, dst);
      //ofxCv::warpPerspective(src, dst, H_cv, cv::INTER_LINEAR); //cv::INTER_NEAREST
    };

  private:

    cv::Mat H_cv;
    ofMatrix4x4 H_of; 
    bool H_ready;

    vector<cv::Point2f> tags_pts;
    vector<cv::Point2f> proj_pts;
    vector<ofVec2f> coords;
    string file_calib;

    ofPixels debug_pix;
    ofTexture debug_tex;

    bool isReady()
    {
      if (!H_ready)
        return false;
      return error() < 0.1;
    };

    //TODO calib calc error H
    float error()
    {
      ofLogNotice() << "error";
      float err = 0;
      for (int i = 0; i < coords.size(); i++)
      {
        ofVec2f tag = toOf(tags_pts[i]);
        ofVec2f tag2;
        transform(tag, tag2);
        ofVec2f proj = nearest_proj_point(tag2);
        float d = tag2.distance(proj);
        err += d;
        ofLogNotice() << "\t tag= " << tag << " tag2= " << tag2 << " nearest proj= " << proj << " d= " << d;
      }
      err /= tags_pts.size();
      ofLogNotice() << "\t err " << err;
      return err;
    };

    ofVec2f nearest_proj_point(ofVec2f point)
    {
      float min_d = std::numeric_limits<float>::max();
      int min = -1;
      for (int i = 0; i < proj_pts.size(); i++)
      {
        float d = point.distance(toOf(proj_pts[i]));
        if (d < min_d)
        {
          min_d = d;
          min = i;
        }
      }
      return toOf(proj_pts[min]);
    };

    ofVec2f tag_coord(ofVec2f& coord, ofVec2f& ctr, vector<ChiliTag>& tags)
    { 
      for (int i = 0; i < tags.size(); i++)
      {
        ofVec2f& t = tags[i].center_n;
        ofVec2f d = t - ctr;
        if (equal_sign(d, coord))
          return t;
      }
    }; 

    ofVec2f tags_ctr(vector<ChiliTag>& tags)
    {
      ofVec2f ctr(0,0);
      for (int i = 0; i < tags.size(); i++)
        ctr += tags[i].center_n; 
      ctr /= tags.size();
      return ctr;
    };

    bool equal_sign(ofVec2f& a, ofVec2f& b)
    {
      ofVec2f s = a * b;
      return s.x > 0 && s.y > 0;
    };

    void debug_tags(vector<ChiliTag>& tags, float x, float y, float w, float h)
    {
      float pw = ofGetWidth();
      float ph = ofGetHeight();
      ofVec2f pscale(pw,ph);

      //ofVec2f scale(w,h);
      //ofVec2f translate(x,y);

      ofPushStyle();
      ofSetColor(ofColor::magenta);

      for (int i = 0; i < tags.size(); i++)
      {
        vector<ofVec2f> &corners_n = tags[i].corners_n;
        ofVec2f p0, p1;
        for (int j = 0; j < corners_n.size(); j++)
        {
          p0 = corners_n[j];
          p1 = corners_n[ (j+1)%4 ]; 

          p0 *= pscale; //* scale + translate;
          p1 *= pscale; //* scale + translate;

          transform(p0, p0);
          transform(p1, p1);

          ofDrawLine( p0.x, p0.y, p1.x, p1.y );
        }
      }

      ofPopStyle();
    };

    void debug_pixels(ofPixels& pix, float x, float y, float w, float h)
    {
      ofPixels pix2; 
      float px = ofGetWidth();
      float py = ofGetHeight();
      float cx = pix.getWidth();
      float cy = pix.getHeight(); 
      ofxCv::imitate(pix, pix2);
      ofxCv::resize(pix, pix2, px/cx, py/cy);
      transform(pix2, debug_pix);
      debug_tex.loadData(debug_pix);
      debug_tex.draw(x, y, w, h);
    };

    //based on ofxQuadWarp
    //https://github.com/julapy/ofxQuadWarp/blob/master/src/ofxQuadWarp.cpp#L203-L229
    void H_cv2of(cv::Mat& src, ofMatrix4x4& dst)
    {
      float* mat = (float*)src.data;

      //we need to copy these values
      //from the 3x3 2D openCV matrix which is row ordered
      //
      // ie:   [0][1][2] x
      //       [3][4][5] y
      //       [6][7][8] w

      //to openGL's 4x4 3D column ordered matrix
      //        x  y  z  w   
      // ie:   [0][3][ ][6]
      //       [1][4][ ][7]
      //		 [ ][ ][ ][ ]
      //       [2][5][ ][9]
      //       

      dst.getPtr()[0]  = mat[0];
      dst.getPtr()[4]  = mat[1];
      dst.getPtr()[12] = mat[2];

      dst.getPtr()[1]  = mat[3];
      dst.getPtr()[5]  = mat[4];
      dst.getPtr()[13] = mat[5];	

      dst.getPtr()[3]  = mat[6];
      dst.getPtr()[7]  = mat[7];
      dst.getPtr()[15] = mat[8];
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
      H_ready = true;
      return true;
    }; 

};

