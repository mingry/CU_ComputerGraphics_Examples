#version 330

layout (location=0) in vec4 vs_position;
layout (location=1) in vec4 vs_color;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

out vec4 fs_color;

void main()
{
	gl_Position = projection_matrix * modelview_matrix * vs_position;
	fs_color = vs_color;
}


