#pragma once

#include "ofxMicromundos/Bloque.h"
#include "GUI.h"

class FlowFieldLayer
{ 

  public:

    FlowFieldLayer() {};
    ~FlowFieldLayer() {}; 

    virtual ofTexture& get() = 0;
    virtual float* get_data() = 0;
    virtual void inject(shared_ptr<GUI> gui) = 0;
    virtual void init(float w, float h) = 0;
    virtual void update(ofTexture& proj_tex, map<int, Bloque>& bloques) = 0;
    virtual void dispose() = 0;
    virtual void render_monitor(float x, float y, float w, float h) = 0;
};

