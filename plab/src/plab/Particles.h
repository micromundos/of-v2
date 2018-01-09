#pragma once

#include "Fisica.h"
#include "CoordMap.h"

class Particles
{ 

  public:

    Particles() {};
    ~Particles() 
    {
      dispose();
    };

    void inject(Fisica* fisica) 
    {
      this->fisica = fisica;
    };

    void init() 
    {
      radius = 6.;
      max_particles = 5000.;
      max_speed = 3.;
      render_size = 4.;
      lifetime = 15.;

      mesh.setMode( OF_PRIMITIVE_POINTS );

      //ofcolor.set( 127, 200, 255 );
      //b2color.Set(ofcolor.r, ofcolor.g, ofcolor.b, ofcolor.a);

      b2ParticleSystemDef psd;
      psd.radius = radius / fisica->scale();
      psd.maxCount = max_particles;

      /// Reduces relative velocity of viscous particles
      /// Larger values slow down viscous particles more
      psd.viscousStrength = 0.6f; //0.25f;

      /// Change the particle density.
      /// Particle density affects the mass of the particles, which in turn
      /// affects how the particles interact with b2Bodies. Note that the density
      /// does not affect how the particles interact with each other.
      //psd.density = 0.0f; //1.0f;

      /// Pushes particles out of solid particle group
      /// Larger values repulse more
      //psd.ejectionStrength = 0.0f; //0.5f;

      b2particles = fisica->b2world()->CreateParticleSystem(&psd);
    }; 

    void update()
    {
      limit_speed();
    };

    void update_flowfield(float* field, ofVec2f& screen_size, ofVec2f& flowfield_size) 
    {
      screen2ff.set( screen_size.x, screen_size.y, flowfield_size.x, flowfield_size.y );

      if (field == nullptr) 
      {
        ofLogWarning("Particles") << "flow field input process has no data";
        return;
      }

      int32 n = b2particles->GetParticleCount();
      b2Vec2 *locs = b2particles->GetPositionBuffer(); 

      b2Vec2 force;
      ofVec2f ff_loc, screen_loc;
      for (int i = 0; i < n; i++)
      {
        b2Vec2& loc = locs[i]; 

        fisica->world2screen( loc, screen_loc );
        screen2ff.dst( screen_loc, ff_loc );

        int fi = ((int)ff_loc.x + (int)ff_loc.y * flowfield_size.x) * 4; //chann:rgba
        force.Set( field[fi], field[fi+1] );

        if ( max_force > 0 )
        {
          float len = force.Normalize();
          force *= len > max_force ? max_force : len;
        }

        b2particles->ParticleApplyForce( i, force );
      }
    };

    void render()
    {
      mesh.clear();

      int32 n = b2particles->GetParticleCount();
      b2Vec2 *locs = b2particles->GetPositionBuffer();
      b2ParticleColor *cols = b2particles->GetColorBuffer();

      ofVec2f loc;
      ofColor col;
      for ( int i = 0; i < n; i++ )
      {
        loc.set( locs[i].x, locs[i].y );
        col.set( cols[i].r, cols[i].g, cols[i].b );
        mesh.addVertex( loc );
        mesh.addColor( col );
      }

      ofPushStyle();
      ofSetColor(255);
      ofSetLineWidth(0.1);
      glPointSize( render_size );
      ofPushMatrix();
      ofScale( fisica->scale(), fisica->scale() );
      mesh.draw();
      ofPopMatrix();
      ofPopStyle();
    };

    void dispose()
    {
      fisica = nullptr;
    };

    int32 make_particle( float _locx, float _locy, float _velx, float _vely, ofColor _color )
    {
      float locx, locy, velx, vely;
      fisica->screen2world(_locx, _locy, locx, locy);
      fisica->screen2world(_velx, _vely, velx, vely);

      uint32 flags = b2_waterParticle | /*b2_springParticle |*/ b2_viscousParticle;

      b2ParticleDef pd;
      pd.flags = flags;
      pd.position.Set( locx, locy );
      pd.velocity.Set( velx, vely );
      b2ParticleColor p_color;
      p_color.Set(_color.r, _color.g, _color.b, 255.0);
      pd.color = p_color;
      if ( lifetime > 0.0 )
        pd.lifetime = lifetime; 

      return b2particles->CreateParticle( pd );
    }; 

    b2ParticleSystem* b2_particles()
    {
      return b2particles;
    }; 

  private:

    Fisica* fisica;
    b2ParticleSystem* b2particles;

    ofVboMesh mesh;
    CoordMap screen2ff;

    //ofColor ofcolor;
    //b2ParticleColor b2color;

    float radius;
    float max_particles;
    float max_speed;
    float max_force;
    float render_size;
    float lifetime; 

    void limit_speed()
    {
      if ( max_speed == 0.0 )
        return;

      int32 n = b2particles->GetParticleCount();
      b2Vec2 *vels = b2particles->GetVelocityBuffer(); 
      for (int i = 0; i < n; i++)
      {
        b2Vec2& vel = vels[i];
        float len = vel.Normalize();
        vel *= len > max_speed ? max_speed : len;
      }
    };

};

