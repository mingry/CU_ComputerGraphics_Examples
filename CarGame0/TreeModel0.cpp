#include "treeModel0.h"
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>
#include "Mesh.h"
#include "BasicShapeObjs.h"


extern int g_shader_id;

///////////////////////////////////////////////
// Tree

GLuint g_tree_vao;
GLuint g_tree_vbo_pos;
GLuint g_tree_vbo_color;
GLuint g_tree_vbo_normal;
GLuint g_tree_index_buf;

static Mesh g_tree_mesh;

void InitTreeModel()
{
	// Element 1, ³ª¹«±âµÕ
	Mesh trunk = glm::scale(glm::vec3(2, 50, 2)) 
				* glm::translate(glm::vec3(0, 0.5f, 0)) 
				* g_cylinder_mesh;
	trunk.SetColor(1, 0, 1, 1);
	
	// Element 2, ³ª¹µÀÙ
	Mesh leaf = glm::rotate(glm::pi<float>()/2, glm::vec3(0, 1, 0)) 
				* glm::rotate(-glm::pi<float>()/6, glm::vec3(1, 0, 0)) 
				* glm::scale(glm::vec3(5, 0.2, 15)) 
				* glm::translate(glm::vec3(0, 0, 1)) 
				* g_cylinder_mesh;
	leaf.SetColor(0, 1, 0, 1);



	// Tree, ³ª¹«±âµÕ+³ª¹µÀÓ+³ª¹«±âµÕ
	glm::mat4 T(1.0);
	g_tree_mesh = trunk;
	T = glm::translate(glm::vec3(0, 50, 0));
	g_tree_mesh += T * leaf;
	T *= glm::rotate(glm::pi<float>()/6, glm::vec3(1, 0, 0));
	g_tree_mesh += T * trunk;






	// Vertex Array Object
	glGenVertexArrays(1, &g_tree_vao);
	glBindVertexArray(g_tree_vao);


	// Position VBO
	glGenBuffers(1, &g_tree_vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_tree_mesh.num_vertices(), g_tree_mesh.points(), GL_STATIC_DRAW);

	// Color VBO
	glGenBuffers(1, &g_tree_vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*g_tree_mesh.num_vertices(), g_tree_mesh.colors(), GL_STATIC_DRAW);

	// Normal VBO
	glGenBuffers(1, &g_tree_vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_tree_mesh.num_vertices(), g_tree_mesh.normals(), GL_STATIC_DRAW);


	// Index Buffer Object
	glGenBuffers(1, &g_tree_index_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_tree_index_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*g_tree_mesh.num_triangles(), g_tree_mesh.triangle_ids(), GL_STATIC_DRAW);


	// Vertex Attribute
	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_color);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_normal);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
}


void DrawTreeModel()
{
	glBindVertexArray(g_tree_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_tree_index_buf);
	glDrawElements(GL_TRIANGLES, g_tree_mesh.num_triangles()*3, GL_UNSIGNED_INT, NULL);
}


void DeleteTreeModel()
{
	glDeleteBuffers(1, &g_tree_vbo_pos);
	glDeleteBuffers(1, &g_tree_vbo_color);
	glDeleteBuffers(1, &g_tree_vbo_normal);
	glDeleteBuffers(1, &g_tree_index_buf);
	glDeleteVertexArrays(1, &g_tree_vao);
}




