#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

/*
 * flow field for transporters
 *
 * in: source and destination bloqs locations
 * out: transportation force
 */

#pragma include "../lib/math.glsl"
#pragma include "../lib/geom.glsl"
#pragma include "../lib/gpgpu.glsl"

uniform vec2 src;
uniform vec2 dst;
uniform float radius;
uniform float force;
uniform int transporters_size;

float nforce(vec2 point, vec2 loc)
{
  vec2 dir = point - loc;
  float d = length(dir);
  return clamp(lerp2d(d, 0., radius, 1., 0.), EPSILON, FLT_MAX);
}

void main() 
{
  if ( transporters_size < 2 )
  {
    gl_FragColor = vec4(0.,0.,0.,1.);
    return;
  }

  vec2 loc = location();

  vec2 norm = normal_point(loc, src, dst);
  float fmult = nforce(norm, loc);
  vec2 f = normalize(dst-src) * fmult * force; 

  gl_FragColor = vec4(f, 0., 1.);
}

uniform sampler2DRect render_input;

void __render__( void ) 
{
    vec2 p2 = gl_TexCoord[0].xy;
    vec3 _in = texture2DRect(render_input, p2).xyz;

    vec3 _out;

    float r = 1.;
    _out = vec3(
      lerp2d( _in.x, -r,r, 0.,1.),
      lerp2d( _in.y, -r,r, 0.,1.),
      0. //lerp2d( _in.z, -r,r, 0.,1.)
    );

    gl_FragColor = vec4( _out, 1.);
}

