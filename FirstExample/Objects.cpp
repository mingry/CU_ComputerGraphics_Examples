
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>




///////////////////////////////////////////////
// Two-Triangle

GLuint g_two_tri_vao;
GLuint g_two_tri_vbo_pos;
GLuint g_two_tri_vbo_color;

const GLint g_two_tri_vertex_num = 6;


// �� �ﰢ�� Mesh�� ��������� VAO, VBO�� �����ϰ� Vertex Attribute�� �����Ѵ�.
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
	// VAO�� �����ϰ� �ڵ����� �ο��� �̸��� g_two_tri_vao�� ��´�.
	glGenVertexArrays(1, &g_two_tri_vao);
	glBindVertexArray(g_two_tri_vao);


	//// Position VBO
	// ������(vertex)�� ��ǥ���� �����ϱ� ���� VBO�� �����ϰ� �ڵ����� �ο��� VBO�� �̸��� g_two_tri_vbo_pos�� ��´�.
	glGenBuffers(1, &g_two_tri_vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, g_two_tri_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_two_tri_vertex_num, vertices, GL_STATIC_DRAW);	// vertices�� �ִ� ������ g_two_tri_vbo_pos�� �ű��.


	//// Color VBO
	// ������(vertex)�� ������ �����ϱ� ���� VBO�� �����ϰ� �ڵ����� �ο��� VBO�� �̸��� g_two_tri_vbo_color�� ��´�.
	glGenBuffers(1, &g_two_tri_vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, g_two_tri_vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_two_tri_vertex_num, color, GL_STATIC_DRAW);	// color�� �ִ� ������ g_two_tri_vbo_color�� �ű��.


	//// Vertex Attribute
	// g_two_tri_vbo_pos�� ���� Vertex Attribute�� �����Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, g_two_tri_vbo_pos);
	// �Ʒ� �� �Լ��� ù ��° �Ķ���� '0'�� g_two_tri_vbo_pos�� vshader.glsl�� fshader.glsl�� layout=0�� �ش��ϴ� ������ ���� ���� �ǹ��Ѵ�.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);

	// g_two_tri_vbo_color�� ���� Vertex Attribute�� �����Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, g_two_tri_vbo_color);
	// �Ʒ� �� �Լ��� ù ��° �Ķ���� '1'�� g_two_tri_vbo_color�� vshader.glsl�� fshader.glsl�� layout=1�� �ش��ϴ� ������ ���� ���� �ǹ��Ѵ�.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}


// �� �ﰢ�� Mesh�� ���� ������ VAO, VBO�� ����� �׸��� �׸���.
void DrawTwoTri()
{
	glBindVertexArray(g_two_tri_vao);

	// ������(Vertex) �� �� ���� �ϳ��� �ﰢ���� �׸���. 
	glDrawArrays(GL_TRIANGLES, 0, g_two_tri_vertex_num);
}


// �� �ﰢ�� Mesh�� ���� ������ VAO, VBO�� ��� �����.
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
	// ������(Vertex)�� ������� ������ ���� Index Array�� �����Ѵ�.
	// Index Array�� ������� ���۸� �����ϰ� �ڵ����� �ο��� �̸��� g_cube_index_buf�� ��´�.
	glGenBuffers(1, &g_cube_index_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_cube_index_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*g_cube_face_num, indices, GL_STATIC_DRAW);	// indices�� �ִ� ������ g_cube_index_buf�� �ű��.
}


void DrawCube()
{
	glBindVertexArray(g_cube_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_cube_index_buf);
	// g_cube_index_buf �� ���ǵ� �������� �ε��� ��ȣ�� ����, �ε��� �� �� ���� �ϳ��� �ﰢ���� �׸���. 
	glDrawElements(GL_TRIANGLES, g_cube_face_num*3, GL_UNSIGNED_INT, NULL);
}


void DeleteCube()
{
	glDeleteBuffers(1, &g_cube_vbo_pos);
	glDeleteBuffers(1, &g_cube_index_buf);
	glDeleteVertexArrays(1, &g_cube_vao);
}