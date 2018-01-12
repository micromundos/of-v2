#pragma once

#include "ofxCv.h"
#include "ofxGPGPU.h"
#include "ofxGPGPU/shaders/gaussian.h"
#include "GUI.h"

using namespace ofxCv;

class FlowField
{ 

  public:

    FlowField() {};
    ~FlowField() 
    {
      dispose();
    }; 

    void inject(shared_ptr<GUI> gui) 
    {
      this->gui = gui;
    };

    void init(float w, float h) 
    {
      ff = nullptr;
      ff_w = w;
      ff_h = h; 

      edges
        .init("glsl/openvision/canny.fs", w, h)
        .on("update", this, &FlowField::update_edges);

      gaussian
        .init(w, h)
        .on("update", this, &FlowField::update_gaussian);

      container
        .init("glsl/flowfields/flowfield_container.frag", w, h)
        .on("update", this, &FlowField::update_container);
    };

    void dispose() 
    {
      ff = nullptr;
      gui = nullptr;

      edges
        .off("update", this, &FlowField::update_edges)
        .dispose();

      gaussian
        .off("update", this, &FlowField::update_gaussian)
        .dispose();

      container
        .off("update", this, &FlowField::update_container)
        .dispose();
    };

    void update(ofTexture& proj_tex, map<int, Bloque>& bloques)
    {
      //TODO flow field passes stack
      //edge container
      //stream flow
      //attractors
      //ff = merge

      //int canny_kernel = 3;
      //double canny_low_thres = 100;
      //double canny_high_thres = 150;
      //cv::Canny(src, edges_mat, canny_low_thres, canny_high_thres, canny_kernel);
      //ofxCv::invert(edges_mat);
      //cv::GaussianBlur(edges_mat, edges_mat, cv::Size(gui->gaussian_kernel, gui->gaussian_kernel), (double)gui->gaussian_sigma);

      edges
        .set("tex", proj_tex)
        .update();

      gaussian
        .set("data", edges.get())
        .update(2); //horiz + vert

      container
        .set("edges", gaussian.get())
        .update()
        .update_render(gui->backend_monitor);

      ff = container.get_data();
    };

    void render_monitor(float x, float y, float w, float h)
    {
      if (!gui->backend_monitor)
        return;

      edges.get().draw(x, y, w, h);
      gaussian.get().draw(x + w, y, w, h);
      container.render(x + w*2, y, w, h);

      //toOf(edges_mat, edges_pix); 
      //if (edges_pix.isAllocated())
        //edges_tex.loadData(edges_pix);
      //if (edges_tex.isAllocated())
        //edges_tex.draw(x + w, y, w, h);
    };

    float* get() { return ff; };
    float width() { return ff_w; };
    float height() { return ff_h; };

  private:

    shared_ptr<GUI> gui;

    float* ff;
    float ff_w;
    float ff_h;

    gpgpu::Process container;
    gpgpu::Process edges;
    gpgpu::Gaussian gaussian; 

    void update_container(ofShader& shader)
    {
      shader.setUniform1f("force_mult", gui->container_force_mult);
    };

    void update_edges(ofShader& shader)
    {
      shader.setUniform1f("threshold", gui->edge_threshold);
    };

    void update_gaussian(ofShader& shader)
    {
      shader.setUniform1f("sigma", gui->gaussian_sigma);
      shader.setUniform1i("kernel", gui->gaussian_kernel);
      shader.setUniform1f("alpha", 1.0);
    }; 
};


