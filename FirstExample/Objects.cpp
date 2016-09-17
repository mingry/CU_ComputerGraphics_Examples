
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>




///////////////////////////////////////////////
// Two-Triangle

GLuint g_two_tri_vao;
GLuint g_two_tri_vbo_pos;
GLuint g_two_tri_vbo_color;

const GLint g_two_tri_vertex_num = 6;


// 두 삼각형 Mesh를 만들기위해 VAO, VBO를 생성하고 Vertex Attribute를 설정한다.
void InitTwoTri()
{
	GLfloat vertices[g_two_tri_vertex_num*3] = { 0.f, 0.f, 0.f
											, 45.f, 0.f, 0.f
											, 0.f, 45.f, 0.f

											, 50.f, 0.f, 0.f
											, 50.f, 50.f, 0.f
											, 5.f, 50.f, 0.f
										};

	GLfloat color[g_two_tri_vertex_num*3] = { 0.f, 0.f, 1.f 
							, 1.f, 0.f, 0.f
							, 0.f, 1.f, 0.f

							, 0.f, 0.f, 1.f
							, 1.f, 0.f, 0.f
							, 0.f, 1.f, 0.f
							};
	//// Vertex Array Object
	// VAO를 생성하고 자동으로 부여된 이름을 g_two_tri_vao에 담는다.
	glGenVertexArrays(1, &g_two_tri_vao);
	glBindVertexArray(g_two_tri_vao);


	//// Position VBO
	// 꼭지점(vertex)의 좌표값을 저장하기 위한 VBO를 생성하고 자동으로 부여된 VBO의 이름을 g_two_tri_vbo_pos에 담는다.
	glGenBuffers(1, &g_two_tri_vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, g_two_tri_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_two_tri_vertex_num, vertices, GL_STATIC_DRAW);	// vertices에 있는 내용을 g_two_tri_vbo_pos로 옮긴다.


	//// Color VBO
	// 꼭지점(vertex)의 색깔값을 저장하기 위한 VBO를 생성하고 자동으로 부여된 VBO의 이름을 g_two_tri_vbo_color에 담는다.
	glGenBuffers(1, &g_two_tri_vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, g_two_tri_vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_two_tri_vertex_num, color, GL_STATIC_DRAW);	// color에 있는 내용을 g_two_tri_vbo_color로 옮긴다.


	//// Vertex Attribute
	// g_two_tri_vbo_pos를 위한 Vertex Attribute를 설정한다.
	glBindBuffer(GL_ARRAY_BUFFER, g_two_tri_vbo_pos);
	// 아래 두 함수의 첫 번째 파라메터 '0'은 g_two_tri_vbo_pos가 vshader.glsl와 fshader.glsl의 layout=0에 해당하는 변수에 연결 됨을 의미한다.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);

	// g_two_tri_vbo_color를 위한 Vertex Attribute를 설정한다.
	glBindBuffer(GL_ARRAY_BUFFER, g_two_tri_vbo_color);
	// 아래 두 함수의 첫 번째 파라메터 '1'은 g_two_tri_vbo_color가 vshader.glsl와 fshader.glsl의 layout=1에 해당하는 변수에 연결 됨을 의미한다.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}


// 두 삼각형 Mesh를 위해 생성된 VAO, VBO를 사용해 그림을 그린다.
void DrawTwoTri()
{
	glBindVertexArray(g_two_tri_vao);

	// 꼭지점(Vertex) 세 개 마다 하나의 삼각형을 그린다. 
	glDrawArrays(GL_TRIANGLES, 0, g_two_tri_vertex_num);
}


// 두 삼각형 Mesh를 위해 생성된 VAO, VBO를 모두 지운다.
void DeleteTwoTri()
{
	glDeleteBuffers(1, &g_two_tri_vbo_pos);
	glDeleteBuffers(1, &g_two_tri_vbo_color);
	glDeleteVertexArrays(1, &g_two_tri_vao);
}






///////////////////////////////////////////////
// Cube

GLuint g_cube_vao;
GLuint g_cube_vbo_pos;
GLuint g_cube_index_buf;

const GLint g_cube_vertex_num = 8;
const GLint g_cube_face_num = 12;

void InitCube()
{
	GLfloat vertices[g_cube_vertex_num*3] = { -50.f, 50.f, 50.f
											, 50.f, 50.f, 50.f 
											, 50.f, 50.f, -50.f 
											, -50.f, 50.f, -50.f 

											, -50.f, -50.f, 50.f 
											, 50.f, -50.f, 50.f 
											, 50.f, -50.f, -50.f 
											, -50.f, -50.f, -50.f 
										};

	GLuint indices[g_cube_face_num*3] = { 0, 4, 5,
									0, 5, 1,
									1, 5, 6,
									1, 6, 2,
									2, 6, 7,
									2, 7, 3,
									3, 7, 4,
									3, 4, 0,
									0, 1, 2,
									0, 2, 3,
									4, 7, 6,
									4, 6, 5 };

	// Vertex Array Object
	glGenVertexArrays(1, &g_cube_vao);
	glBindVertexArray(g_cube_vao);


	// Vertex Buffer Object
	glGenBuffers(1, &g_cube_vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, g_cube_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_cube_vertex_num, vertices, GL_STATIC_DRAW);

	
	// Vertex Attribute
	glBindBuffer(GL_ARRAY_BUFFER, g_cube_vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Index Buffer Object
	// 꼭지점(Vertex)의 연결관계 설명을 위한 Index Array를 설정한다.
	// Index Array를 담기위한 버퍼를 생성하고 자동으로 부여된 이름을 g_cube_index_buf에 담는다.
	glGenBuffers(1, &g_cube_index_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_cube_index_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*g_cube_face_num, indices, GL_STATIC_DRAW);	// indices에 있는 내용을 g_cube_index_buf로 옮긴다.
}


void DrawCube()
{
	glBindVertexArray(g_cube_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_cube_index_buf);
	// g_cube_index_buf 에 정의된 꼭지점의 인덱스 번호를 보고, 인덱스 세 개 마다 하나의 삼각형을 그린다. 
	glDrawElements(GL_TRIANGLES, g_cube_face_num*3, GL_UNSIGNED_INT, NULL);
}


void DeleteCube()
{
	glDeleteBuffers(1, &g_cube_vbo_pos);
	glDeleteBuffers(1, &g_cube_index_buf);
	glDeleteVertexArrays(1, &g_cube_vao);
}