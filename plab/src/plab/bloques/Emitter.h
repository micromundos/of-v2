#pragma once

#include "ofxMicromundos/Bloque.h"
#include "plab/bloques/BloqueProcess.h"

class Emitter : public BloqueProcess
{ 
  public:

    Emitter() {};
    ~Emitter() 
    {
      dispose();
    };

    void dispose() 
    {
      BloqueProcess::dispose(); 
    }; 

    void init() 
    {
      initial_fps = fisica->fps();
      emit_remainder = 0.;
      emit_rate = 10.;
      emit_force = 15.;
    };

    void update(map<int, Bloque>& bloques) 
    { 
      // How many (fractional) particles should we have emitted this frame?
      float	dt = (1.0f / initial_fps);
      //float	dt = (1.0f / fisica->fps());
      //float	dt = (1.0f / ofGetFrameRate());
      emit_remainder += emit_rate * dt;

      // Keep emitting particles on this frame until we only have a fractional particle left.
      //while (emit_remainder > 1.0f) 
      //{
        //emit_remainder -= 1.0f;
        //emit(bloq, render_data);
      //}
    };

    void render()
    {
    };

  private:

    float initial_fps;
    float emit_remainder;
    float emit_rate;
    float emit_force;


    //void emit( Bloq* bloq, RenderComponent* render_data )
    //{
      //ParticleSystem* ps = system<ParticleSystem>();
      //b2ParticleSystem* b2ps = ps->b2_particles();

      //ofVec2f screen_loc( bloq->loc.x * render_data->width, bloq->loc.y * render_data->height );

      //int32 pidx = ps->make_particle( screen_loc.x, screen_loc.y, 0, 0 , bloq->id == "1" ? ofColor(0,140,140) : ofColor(140,0,140));

      //float force_m = emit_force;

      ////ofVec2f force = bloq->dir * force_m;
      //b2Vec2 force( bloq->dir.x, bloq->dir.y );
      //force *= force_m;
      //b2ps->ParticleApplyForce( pidx, force );
    //};
};

