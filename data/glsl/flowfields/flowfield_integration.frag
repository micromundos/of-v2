#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

/*
 * flowfield
 *
 * in: list of flowfields, all same size
 * out: integration of flowfields
 */

#pragma include "../lib/math.glsl"
#pragma include "../lib/gpgpu.glsl"

uniform int n_layers;

uniform float weight_0;
uniform float weight_1;
uniform float weight_2;
uniform float weight_3;

/*https://www.opengl.org/wiki/Data_Type_%28GLSL%29#Opaque_arrays*/
uniform sampler2DRect layer_0;
uniform sampler2DRect layer_1;
uniform sampler2DRect layer_2;
uniform sampler2DRect layer_3;

void main( void ) 
{
  vec2 force = vec2(0.,0.);

  if (n_layers > 0) force += texel( layer_0 ).xy * weight_0;
  if (n_layers > 1) force += texel( layer_1 ).xy * weight_1;
  if (n_layers > 2) force += texel( layer_2 ).xy * weight_2;
  if (n_layers > 3) force += texel( layer_3 ).xy * weight_3;

  gl_FragColor = vec4( force, 0., 1. );
}


uniform sampler2DRect render_input;

void __render__( void ) 
{
    vec3 _in = texel(render_input).xyz;

    vec3 _out;

    float r = 1.;
    _out = vec3(
      lerp2d( _in.x, -r,r, 0.,1.),
      lerp2d( _in.y, -r,r, 0.,1.),
      lerp2d( _in.z, -r,r, 0.,1.)
    );

    gl_FragColor = vec4( _out, 1.);
}

