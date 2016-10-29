#version 330

layout (location=0) in vec4 vs_position;
layout (location=1) in vec4 vs_color;
layout (location=2) in vec4 vs_normal;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

out vec4 fs_color;
out vec4 fs_transformed_normal;


void main()
{
	gl_Position = projection_matrix * modelview_matrix * vs_position;

	fs_transformed_normal = modelview_matrix * vec4(vs_normal.xyz,0);
	fs_color = vs_color;
}


