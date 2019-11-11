#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

/*
 * TODO glsl apply homography
 * https://stackoverflow.com/questions/33018652/how-to-use-opengl-to-emulate-opencvs-warpperspective-functionality-perspective
 * H_inverse = inverse homography
 * use glUniformMatrix3fv(loc, count, transpose=true, H);
 */

#pragma include "../lib/gpgpu.glsl"

uniform sampler2DRect input;
uniform mat3 H_inverse;

void main( void ) 
{
  vec3 coord = vec3(location(input), 1.0);

  // Determine what 'z' is
  vec3 m = H_inverse[2] * coord;
  float zed = 1.0 / (m.x + m.y + m.z);
  coord *= zed;

  // Determine translated x and y coordinates
  vec2 transformed = vec2(
      H_inverse[0][0] * coord.x + H_inverse[0][1] * coord.y + H_inverse[0][2] * coord.z,
      H_inverse[1][0] * coord.x + H_inverse[1][1] * coord.y + H_inverse[1][2] * coord.z);

  if (transformed.x >= 0.0 
      && transformed.x < size.x 
      && transformed.y >= 0.0 
      && transformed.y < size.y) 
  {
    gl_FragColor = texel(input, transformed);
  } 
  else 
  {
    gl_FragColor = vec4(0.0);
  }
}
