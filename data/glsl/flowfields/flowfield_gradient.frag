#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

/*
 * flowfield
 *
 * in: data
 *  height map in mm
 * out: 
 *  2d vector field
 */

#pragma include "../lib/math.glsl"
#pragma include "../lib/gpgpu.glsl"

uniform sampler2DRect height_map;
uniform float max_force;

const int kernel = 3;

void main( void ) 
{
  vec2 height_map_size = texsize(height_map);

  vec2 loc_height_map = location() * height_map_size;

  float height = texel(height_map,loc_height_map).r;

  if ( height < 0.0 )
  {
    gl_FragColor = vec4(0.,0.,0.,1.);
    return;
  }

  vec2 force = vec2(0.,0.);

  int n = 0;
  int ini = -kernel / 2;
  int end = -ini;

  for ( int i = ini; i <= end; i++ )
  for ( int j = ini; j <= end; j++ )
  {
    if ( i == 0 && j == 0 )
      continue; 

    vec2 ndir = vec2(i,j);
    vec2 nloc_height_map = loc_height_map + ndir;

    if ( nloc_height_map.x < 0 || nloc_height_map.x >= height_map_size.x || nloc_height_map.y < 0 || nloc_height_map.y >= height_map_size.y )
      continue;

    float nheight = texel( height_map, nloc_height_map ).r;

    if ( nheight < 0.0 )
      continue;

    // drive away from table or towards table
    float slope = nheight - height;
    force += normalize(ndir) * slope;
    n++;
  }

  if (n > 0) 
  {
    force /= n; 
  }

  // limit force
  float len = length(force);
  if ( len > max_force )
  {
    force = (force/len) * max_force;
  }

  gl_FragColor = vec4( force, 0.,1.);
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

