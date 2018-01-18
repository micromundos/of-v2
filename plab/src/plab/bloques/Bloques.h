#pragma once

#include "plab/bloques/BloqueProcess.h"
#include "plab/Fisica.h"
#include "plab/Particles.h"

class Bloques
{ 
  public:

    Bloques() {};
    ~Bloques() 
    {
      dispose();
    };

    void dispose() 
    {
      fisica = nullptr; 
      particles = nullptr;

      for (int i = 0; i < procs.size(); i++)
        procs[i]->dispose();
      procs.clear(); //TODO will destroy shared_ptr's ???
    };

    void add(shared_ptr<BloqueProcess> bloque_proc)
    {
      procs.push_back(bloque_proc);
    };

    void inject(Fisica* fisica, Particles* particles)
    {
      this->fisica = fisica;
      this->particles = particles;
    };

    void init()
    {
      for (int i = 0; i < procs.size(); i++)
      {
        procs[i]->inject(fisica, particles);
        procs[i]->init();
      }
    };

    void update(map<int, Bloque>& bloques)
    {
      for (int i = 0; i < procs.size(); i++)
        procs[i]->update(bloques);
    };

    void render()
    {
      for (int i = 0; i < procs.size(); i++)
        procs[i]->render();
    };

  private:

    vector<shared_ptr<BloqueProcess>> procs;

    Fisica* fisica;
    Particles* particles;

};

