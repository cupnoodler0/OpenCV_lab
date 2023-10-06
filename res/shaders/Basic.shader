#shader vertex
#version 330 core

// layout(location = 0) in vec3 position;
// Was in tuto
layout(location = 0) in vec4 position; 
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
  gl_Position = u_MVP * position;//vec4(position, 1.0);
  v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_Time;

void main()
{
  float time = clamp(u_Time*100, 0, 1);

  vec4 texColor = texture(u_Texture, v_TexCoord);
  // texColor = texColor * u_Time;
  color = vec4(texColor.r - sin(u_Time/2)/2,
               texColor.g,
               texColor.b,
               texColor.a);
};