#version 330
				
in vec4 fs_color;
	
layout (location = 0) out vec4 color;

void main()
{
	color = fs_color;
}

