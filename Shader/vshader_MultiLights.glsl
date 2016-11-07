#version 330

layout (location=0) in vec4 vs_position;	// 꼭지점 위치
layout (location=1) in vec4 vs_color;		// 꼭지점 색깔
layout (location=2) in vec4 vs_normal;		// 꼭지점 노말벡터

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

out vec4 fs_color;
out vec3 fs_transformed_normal;	// 변환 행렬이 적용된 노말벡터.
out vec3 fs_eye_dir;	// 보는 방향 (꼭지점의 위치와 같다).


void main()
{
	gl_Position = projection_matrix * modelview_matrix * vs_position;

	fs_eye_dir = (modelview_matrix * vs_position).xyz;	// Projection을 적용하지 않음.
	fs_transformed_normal = (modelview_matrix * vec4(vs_normal.xyz,0)).xyz;	// 노말벡터에는 이동 변환(Tanslation)을 적용하지 않음.

	fs_color = vs_color;
}

