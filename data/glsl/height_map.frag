#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

/*
 * in: 
 *  3d point cloud data in metric space
 * out: 
 *  height map in mm
 */

#pragma include "lib/math.glsl"
#pragma include "lib/geom.glsl"
#pragma include "lib/gpgpu.glsl"

uniform sampler2DRect point_cloud;
uniform sampler2DRect render_input;
uniform vec4 plane;

void main( void ) 
{
  vec3 p3 = texel( point_cloud ).xyz;
  float height = plane_distance( plane, p3 );
  gl_FragColor = vec4( height, height, height, 1. );
}

void __render__( void ) 
{
  float hmax = 300.;
  float _in = texel( render_input ).r;
  float _out = _in > hmax ? 0. : lerp2d( _in, 0., hmax, 0., 1. );
  /*float _out = lerp2d( _in, 0., hmax, 0., 1. );*/
  /*float _out = _in == 0. ? 0. : lerp2d( _in, 0., 5000., 0.2, 1. );*/
  gl_FragColor = vec4( _out, _out, _out, 1. );
}


