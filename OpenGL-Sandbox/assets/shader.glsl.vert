#version 450 core

layout (location = 0) in vec3 a_Position;  // Position 0 is for position vector
layout (location = 1) in vec4 a_Color;     // Position 1 is for color vector
layout (location = 2) in vec2 a_TexCoord;  // Position 2 is for texture coordinates
layout (location = 3) in float a_TexIndex; // Position 2 is for texture Id

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoord;
flat out float v_TexIndex;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	gl_Position = u_ViewProj * vec4(a_Position, 1.0f);
}