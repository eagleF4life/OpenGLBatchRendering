#version 450 core

layout (location = 0) in vec3 a_Position; // Position 0 is for Position vector
layout (location = 1) in vec4 a_Color;    // Position 1 is for color vector

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = u_ViewProj * vec4(a_Position, 1.0f);
}