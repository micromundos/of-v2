#pragma once

using namespace ofxCv;

class Segmentation
{
  public:

    Segmentation() {};
    ~Segmentation() 
    {
      dispose();
    };

    void init()
    {
    };

    void update(ofPixels& pix)
    {
      ofxCv::copyGray(pix, bin_mat);
      ofxCv::autothreshold(bin_mat, false);
      ofxCv::toOf(bin_mat, bin_pix); 
    };

    void render(float x, float y, float w, float h)
    {
      bin_tex.loadData(bin_pix);
      bin_tex.draw(x, y, w, h);
    };

    void dispose()
    {
      bin_pix.clear();
      bin_tex.clear();
    };

  private:

    cv::Mat bin_mat;
    ofPixels bin_pix;
    ofTexture bin_tex;
};

