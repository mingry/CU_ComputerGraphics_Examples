#version 330
				
in vec4 fs_color;
in vec3 fs_transformed_normal;
in vec3 fs_eye_dir;

layout (location = 0) out vec4 color;

uniform vec3 light_position;	// Point Light ���� (���� ����ϴ� ��ġ)
float I_l;       // light intensity
float I_a = 1.0; // ambient light intensity

uniform float ambient_factor = 0.25f;
uniform float shininess_n;	
uniform vec3 K_s;

void main()
{
	// Phong Shading ��� (���� �ڷ� ����)
	vec3 K_a = ambient_factor * fs_color.rgb;
	vec3 K_d = fs_color.rgb;

	vec3 L = normalize(light_position-fs_eye_dir);
	vec3 N = normalize(fs_transformed_normal);
	vec3 V = normalize(-fs_eye_dir);
	vec3 H = (L+V)/length(L+V);

	float d = length(light_position-fs_eye_dir);

	// I_l: Light intensity
	if ( d > 0.00001f )
		I_l = min(1.0f/(0.00001*d*d), 1.0f);	
	else
		I_l = 1.f;
	
	vec3 ambient = I_a * K_a;
	vec3 diffuse = I_l * K_d * max(0.f, dot(L, N));
	vec3 specular = I_l * K_s * pow(max(0.f, dot(N, H)), shininess_n);


	color.rgb = ambient + diffuse + specular;
	color.rgb = min(vec3(1.f, 1.f, 1.f), color.rgb);
	color.a = 1.0f;
}

