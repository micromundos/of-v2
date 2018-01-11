#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

/*
 * bilateral filter
 * code by
 * Nicolas Seiller 
 * http://image.inha.ac.kr/oopgpu/documentation/_g_l_s_l_bilateral_processor_8h_source.html
 */

#pragma include "../lib/gpgpu.glsl"

uniform sampler2DRect data;  

uniform float domain_sigma;
uniform float range_sigma;
uniform int kernel;
/*const float domain_sigma = 10.0; //10.0;*/
/*const float range_sigma = 2.0; //0.2;*/
/*const int kernel = 6;*/

const float PI = 3.14159265f;

void main()
{
  vec2 loc = location(data);
  vec3 curRGB = texture2DRect( data, loc ).rgb;

  vec3 graySum = vec3(0.0);
  vec3 intenSum = vec3(0.0);

  vec3 inten = vec3(0.0);
  vec3 sample = vec3(0.0);

  vec3 buf = vec3(0.0);

  float domain_sigma_2 = domain_sigma * domain_sigma;  
  float range_sigma_2 = range_sigma * range_sigma;  

  int start_id = -kernel / 2;
  int stop_id = -start_id;

  float dist;

  for ( int i = start_id ; i <= stop_id; i++ )
  for ( int j = start_id; j <= stop_id; j++ )
  {

    sample = texture2DRect( data, loc + vec2(i,j) ).rgb;
    dist = float(i*i) + float(j*j);

    inten = exp( (-1.0 * dist / (2.0 * domain_sigma_2))) / ( 2.0f * PI * domain_sigma_2) * exp( (-1.0 * (curRGB - sample) * (curRGB - sample) / (range_sigma_2)) ) ;

    intenSum += inten;
    graySum += inten * sample;
  }

  buf = graySum / intenSum;  

  gl_FragColor = vec4( buf, 1.0 );
} 

