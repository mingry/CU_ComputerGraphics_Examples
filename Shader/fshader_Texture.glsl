#version 330
				
in vec4 fs_color;
in vec2 fs_uv;
in vec4 fs_transformed_normal;


layout (location = 0) out vec4 color;

uniform vec3 light_direction = vec3(1.0, 1.0, 1.0);

uniform sampler2D tex0;
uniform bool flag_texture;

void main()
{
	vec3 n = normalize(fs_transformed_normal.xyz);
	vec3 l = normalize(light_direction);
	float diffuse_intensity = dot(n , l);


	color[0] = fs_color[0]*diffuse_intensity;
	color[1] = fs_color[1]*diffuse_intensity;
	color[2] = fs_color[2]*diffuse_intensity;
	color[3] = 1.0f;

	if ( flag_texture )
	{
		color = diffuse_intensity*texture(tex0, fs_uv);
	}
}

