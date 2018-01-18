#pragma once

#include "ofxMicromundos/Bloque.h"
#include "plab/Fisica.h"
#include "plab/Particles.h"

class BloqueProcess
{ 

  public:

    BloqueProcess() {};
    ~BloqueProcess() {}; 

    virtual void init() = 0;
    virtual void render() = 0;
    virtual void update(map<int, Bloque>& bloques) = 0;

    void dispose() 
    {
      fisica = nullptr; 
      particles = nullptr;
    }; 

    void inject(Fisica* fisica, Particles* particles) 
    {
      this->fisica = fisica;
      this->particles = particles;
    };

  protected:

    Fisica* fisica;
    Particles* particles;
};

