#version 330
				
in vec4 fs_color;
in vec4 fs_transformed_normal;
in vec3 fs_eye_dir;

layout (location = 0) out vec4 color;

uniform vec3 light_direction = vec3(1.0, 1.0, 1.0);
vec3 blue = vec3(0.7, 0.7, 1);

uniform int shading_mode;

void main()
{
	vec3 L = normalize(light_direction);
	vec3 N = normalize(fs_transformed_normal.xyz);
	vec3 V = normalize(-fs_eye_dir);

	float diffuse_intensity = max(0.f, dot(N , L));

	if ( shading_mode == 0 )
	{
		color = diffuse_intensity * fs_color;
		
	}
	else if ( shading_mode == 1 )
	{	
		if (diffuse_intensity > 0.95)      color = vec4(1.0,1.0,1.0,1.0);
		else if (diffuse_intensity > 0.5)  color = vec4(0.5,0.6,0.9,1.0);
		else if (diffuse_intensity > 0.25) color = vec4(0.2,0.3,0.5,1.0);
		else  color = vec4(0.2,0.1,0.1,1.0);
	}
	else if ( shading_mode == 2 )
	{	
		color = vec4(0.,0.,0.,0.5);
	}

	

}

