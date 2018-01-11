#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

//defaults
uniform vec2 size;
uniform int pass;

uniform sampler2DRect tex;

void main()
{
  vec2 loc = gl_TexCoord[0].xy / size * vec2(textureSize2DRect(tex,0));

  vec3 a = texture2DRect(tex, loc + vec2(0, -1)).rgb;
  vec3 b = texture2DRect(tex, loc + vec2(-1, 0)).rgb;
  vec3 c = texture2DRect(tex, loc + vec2(0, 1)).rgb;
  vec3 d = texture2DRect(tex, loc + vec2(0, 1)).rgb;
  vec3 color = min(min(a, b), min(c, d));
  float alpha = texture2DRect(tex, gl_TexCoord[0].xy).a;
  gl_FragColor = vec4(color,alpha);
}
