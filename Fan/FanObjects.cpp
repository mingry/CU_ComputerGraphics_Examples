
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <iostream>


// Head
GLuint g_head_vao;
GLuint g_head_vbo_pos;
GLuint g_head_vbo_color;
GLuint g_head_index_buffer;
const GLint g_head_vertex_num = 24;
const GLint g_head_face_num = 12;


// Neck
GLuint g_neck_vao;
GLuint g_neck_vbo_pos;
GLuint g_neck_vbo_color;
GLuint g_neck_index_buffer;
const GLint g_neck_vertex_num = 16;
const GLint g_neck_face_num = 8;

// Base
GLuint g_base_vao;
GLuint g_base_vbo_pos;
GLuint g_base_vbo_color;
GLuint g_base_index_buffer;
const GLint g_base_vertex_num = 20;
const GLint g_base_face_num = 10;

// Motor
GLuint g_motor_vao;
GLuint g_motor_vbo_pos;
const GLint g_motor_vertex_num = 4;



// Wing
GLuint g_wing_vao;
GLuint g_wing_vbo_pos;
const GLint g_wing_vertex_num = 3;


void InitFanObjects()
{
	// Head
	{ 
		float head_size = 20.f;
		float head_size_half = head_size/2.f;

		GLfloat vertices[g_head_vertex_num*3] = {
			-head_size_half, -head_size_half,  head_size_half
			, -head_size_half, -head_size_half, -head_size_half
			, head_size_half, -head_size_half, -head_size_half
			, head_size_half, -head_size_half,  head_size_half

			, -head_size_half,  head_size_half,  head_size_half
			, -head_size_half,  head_size_half, -head_size_half
			, -head_size_half, -head_size_half, -head_size_half
			, -head_size_half, -head_size_half,  head_size_half
			
			, head_size_half, -head_size_half,  head_size_half
			, head_size_half, -head_size_half, -head_size_half
			, head_size_half,  head_size_half, -head_size_half
			, head_size_half,  head_size_half,  head_size_half
			
			, -head_size_half,  head_size_half, head_size_half
			, -head_size_half, -head_size_half, head_size_half
			,  head_size_half, -head_size_half, head_size_half
			,  head_size_half,  head_size_half, head_size_half
			
			, -head_size_half, -head_size_half, -head_size_half
			, -head_size_half,  head_size_half, -head_size_half
			,  head_size_half,  head_size_half, -head_size_half
			,  head_size_half, -head_size_half, -head_size_half
			
			, -head_size_half, head_size_half,  head_size_half
			,  head_size_half, head_size_half,  head_size_half
			,  head_size_half, head_size_half, -head_size_half
			, -head_size_half, head_size_half, -head_size_half
											};

		GLfloat color[g_head_vertex_num*3] = { 
			1.f, 0.f, 0.f,	1.f, 0.f, 0.f,	1.f, 0.f, 0.f,	1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,	0.f, 1.f, 0.f,	0.f, 1.f, 0.f,	0.f, 1.f, 0.f,
			0.f, 0.f, 1.f,  0.f, 0.f, 1.f,  0.f, 0.f, 1.f,  0.f, 0.f, 1.f,
			1.f, 1.f, 0.f,  1.f, 1.f, 0.f,  1.f, 1.f, 0.f,  1.f, 1.f, 0.f,
			1.f, 0.f, 1.f,  1.f, 0.f, 1.f,  1.f, 0.f, 1.f,  1.f, 0.f, 1.f,
			0.f, 1.f, 1.f,  0.f, 1.f, 1.f,  0.f, 1.f, 1.f,  0.f, 1.f, 1.f
								};


		GLuint indices[g_head_face_num*3] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20,
		};


		// Vertex Array Object
		glGenVertexArrays(1, &g_head_vao);
		glBindVertexArray(g_head_vao);


		// Position VBO
		glGenBuffers(1, &g_head_vbo_pos);
		glBindBuffer(GL_ARRAY_BUFFER, g_head_vbo_pos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_head_vertex_num, vertices, GL_STATIC_DRAW);


		// Color VBO
		glGenBuffers(1, &g_head_vbo_color);
		glBindBuffer(GL_ARRAY_BUFFER, g_head_vbo_color);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_head_vertex_num, color, GL_STATIC_DRAW);


		// Vertex Attribute
		glBindBuffer(GL_ARRAY_BUFFER, g_head_vbo_pos);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, g_head_vbo_color);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		// Index Buffer Object
		glGenBuffers(1, &g_head_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_head_index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*g_head_face_num, indices, GL_STATIC_DRAW);
	}




	// Neck
	{
		float neck_width_half = 5.f;
		float neck_height_half = 30.f;

		GLfloat vertices[g_neck_vertex_num*3] = {
			  -neck_width_half, -neck_width_half,  neck_height_half
			, -neck_width_half, -neck_width_half, -neck_height_half
			,  neck_width_half, -neck_width_half, -neck_height_half
			,  neck_width_half, -neck_width_half,  neck_height_half

			, -neck_width_half,  neck_width_half,  neck_height_half
			, -neck_width_half,  neck_width_half, -neck_height_half
			, -neck_width_half, -neck_width_half, -neck_height_half
			, -neck_width_half, -neck_width_half,  neck_height_half

			, neck_width_half, -neck_width_half,  neck_height_half
			, neck_width_half, -neck_width_half, -neck_height_half
			, neck_width_half,  neck_width_half, -neck_height_half
			, neck_width_half,  neck_width_half,  neck_height_half
			
			, -neck_width_half, neck_width_half,  neck_height_half
			,  neck_width_half, neck_width_half,  neck_height_half
			,  neck_width_half, neck_width_half, -neck_height_half
			, -neck_width_half, neck_width_half, -neck_height_half
				};

		GLfloat color[g_neck_vertex_num*3] = { 
			0.f, 1.f, 0.f,	0.f, 1.f, 0.f,	0.f, 1.f, 0.f,	0.f, 1.f, 0.f,
			1.f, 0.f, 0.f,	1.f, 0.f, 0.f,	1.f, 0.f, 0.f,	1.f, 0.f, 0.f,
			0.f, 1.f, 1.f,  0.f, 1.f, 1.f,  0.f, 1.f, 1.f,  0.f, 1.f, 1.f,
			1.f, 0.f, 1.f,  1.f, 0.f, 1.f,  1.f, 0.f, 1.f,  1.f, 0.f, 1.f
								};


		GLuint indices[g_neck_face_num*3] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12
		};

		// Vertex Array Object
		glGenVertexArrays(1, &g_neck_vao);
		glBindVertexArray(g_neck_vao);


		// Position VBO
		glGenBuffers(1, &g_neck_vbo_pos);
		glBindBuffer(GL_ARRAY_BUFFER, g_neck_vbo_pos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_neck_vertex_num, vertices, GL_STATIC_DRAW);


		// Color VBO
		glGenBuffers(1, &g_neck_vbo_color);
		glBindBuffer(GL_ARRAY_BUFFER, g_neck_vbo_color);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_neck_vertex_num, color, GL_STATIC_DRAW);


		// Vertex Attribute
		glBindBuffer(GL_ARRAY_BUFFER, g_neck_vbo_pos);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, g_neck_vbo_color);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// Index Buffer Object
		glGenBuffers(1, &g_neck_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_neck_index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*g_neck_face_num, indices, GL_STATIC_DRAW);
	}




	// Base
	{
		float top_width_half = 25.f;
		float bottom_height_half = 30.f;
		float height_half = 5.f;

		GLfloat vertices[g_base_vertex_num*3] = {
			  -top_width_half,  height_half,  top_width_half        
			, -top_width_half,  height_half, -top_width_half        
			, -bottom_height_half, -height_half, -bottom_height_half
			, -bottom_height_half, -height_half,  bottom_height_half

			, bottom_height_half, -height_half,  bottom_height_half
			, bottom_height_half, -height_half, -bottom_height_half
			, top_width_half,  height_half, -top_width_half        
			, top_width_half,  height_half,  top_width_half        

			, -top_width_half,  height_half, top_width_half        
			, -bottom_height_half, -height_half, bottom_height_half
			,  bottom_height_half, -height_half, bottom_height_half
			,  top_width_half,  height_half, top_width_half        
			
			, -bottom_height_half, -height_half, -bottom_height_half
			, -top_width_half,  height_half, -top_width_half        
			,  top_width_half,  height_half, -top_width_half        
			,  bottom_height_half, -height_half, -bottom_height_half

			, -top_width_half, height_half,  top_width_half
			,  top_width_half, height_half,  top_width_half
			,  top_width_half, height_half, -top_width_half
			, -top_width_half, height_half, -top_width_half
				};

		GLfloat color[g_base_vertex_num*3] = { 
			0.f, 1.f, 0.f,	0.f, 1.f, 0.f,	0.f, 1.f, 0.f,	0.f, 1.f, 0.f,
			1.f, 0.f, 0.f,	1.f, 0.f, 0.f,	1.f, 0.f, 0.f,	1.f, 0.f, 0.f,
			1.f, 1.f, 0.f,  1.f, 1.f, 0.f,  1.f, 1.f, 0.f,  1.f, 1.f, 0.f,
			1.f, 0.f, 1.f,  1.f, 0.f, 1.f,  1.f, 0.f, 1.f,  1.f, 0.f, 1.f,
			0.f, 1.f, 1.f,  0.f, 1.f, 1.f,  0.f, 1.f, 1.f,  0.f, 1.f, 1.f
								};


		GLuint indices[g_base_face_num*3] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16
		};

		// Vertex Array Object
		glGenVertexArrays(1, &g_base_vao);
		glBindVertexArray(g_base_vao);


		// Position VBO
		glGenBuffers(1, &g_base_vbo_pos);
		glBindBuffer(GL_ARRAY_BUFFER, g_base_vbo_pos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_base_vertex_num, vertices, GL_STATIC_DRAW);


		// Color VBO
		glGenBuffers(1, &g_base_vbo_color);
		glBindBuffer(GL_ARRAY_BUFFER, g_base_vbo_color);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_base_vertex_num, color, GL_STATIC_DRAW);


		// Vertex Attribute
		glBindBuffer(GL_ARRAY_BUFFER, g_base_vbo_pos);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, g_base_vbo_color);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// Index Buffer Object
		glGenBuffers(1, &g_base_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_base_index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*g_base_face_num, indices, GL_STATIC_DRAW);
	}



	// Motor
	{
		float axis_len = 5.f;
		float width = 10.f;

		GLfloat vertices[g_motor_vertex_num*3] = {
			  0, width, 0
			, 0, width+axis_len, 0
			, -5, width+axis_len, 0    
			, 5, width+axis_len, 0
				};


		// Vertex Array Object
		glGenVertexArrays(1, &g_motor_vao);
		glBindVertexArray(g_motor_vao);


		// Position VBO
		glGenBuffers(1, &g_motor_vbo_pos);
		glBindBuffer(GL_ARRAY_BUFFER, g_motor_vbo_pos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_motor_vertex_num, vertices, GL_STATIC_DRAW);


		// Vertex Attribute
		glBindBuffer(GL_ARRAY_BUFFER, g_motor_vbo_pos);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

	}


	// Wing
	{
		float wing_len = 45.f;

		GLfloat vertices[g_wing_vertex_num*3] = {
			  0, 0, 0
			, wing_len/3, 5, -wing_len
			, -wing_len/3, -5, -wing_len
				};


		// Vertex Array Object
		glGenVertexArrays(1, &g_wing_vao);
		glBindVertexArray(g_wing_vao);


		// Position VBO
		glGenBuffers(1, &g_wing_vbo_pos);
		glBindBuffer(GL_ARRAY_BUFFER, g_wing_vbo_pos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_wing_vertex_num, vertices, GL_STATIC_DRAW);


		// Vertex Attribute
		glBindBuffer(GL_ARRAY_BUFFER, g_wing_vbo_pos);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

	}
}



void DrawHead()
{
	glBindVertexArray(g_head_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_head_index_buffer);
	glDrawElements(GL_TRIANGLES, g_head_face_num*3, GL_UNSIGNED_INT, 0);
}


void DrawMotor()
{
	glBindVertexArray(g_motor_vao);
	glLineWidth(3);

	// Color
	glVertexAttrib4f(1, 0.0f, 0.0f, 0.0f, 1.0f);

	glDrawArrays(GL_LINES, 0, g_motor_vertex_num);
}


void DrawNeck()
{
	glBindVertexArray(g_neck_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_neck_index_buffer);
	glDrawElements(GL_TRIANGLES, g_neck_face_num*3, GL_UNSIGNED_INT, 0);
}


void DrawBase()
{
	glBindVertexArray(g_base_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_base_index_buffer);
	glDrawElements(GL_TRIANGLES, g_base_face_num*3, GL_UNSIGNED_INT, 0);
}

void DrawWing(int color_index)
{
	glBindVertexArray(g_wing_vao);

	// Color
	if ( color_index%3 == 0 )	    glVertexAttrib4f(1, 1.0f, 0.0f, 0.0f, 1.0f);
	else if ( color_index%3 == 1 )	glVertexAttrib4f(1, 0.0f, 1.0f, 0.0f, 1.0f);
	else if ( color_index%3 == 2 )	glVertexAttrib4f(1, 0.0f, 0.0f, 1.0f, 1.0f);
	

	glDrawArrays(GL_TRIANGLES, 0, g_wing_vertex_num);
}




void DeleteFanObjects()
{
	glDeleteBuffers(1, &g_head_vbo_pos);
	glDeleteBuffers(1, &g_head_vbo_color);
	glDeleteBuffers(1, &g_head_index_buffer);
	glDeleteVertexArrays(1, &g_head_vao);

	glDeleteBuffers(1, &g_neck_vbo_pos);
	glDeleteBuffers(1, &g_neck_vbo_color);
	glDeleteBuffers(1, &g_neck_index_buffer);
	glDeleteVertexArrays(1, &g_neck_vao);

	glDeleteBuffers(1, &g_base_vbo_pos);
	glDeleteBuffers(1, &g_base_vbo_color);
	glDeleteBuffers(1, &g_base_index_buffer);
	glDeleteVertexArrays(1, &g_base_vao);

	glDeleteBuffers(1, &g_motor_vbo_pos);
	glDeleteVertexArrays(1, &g_motor_vao);

	glDeleteBuffers(1, &g_wing_vbo_pos);
	glDeleteVertexArrays(1, &g_wing_vao);

}