
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>
#include "Mesh.h"
#include "BasicShapeObjs.h"
#include <string>
#include <stack>

static float g_d = 5.f;
static float g_delta = (20.f/180.f) * glm::pi<float>();
static std::string g_initial_str = "X";



std::string Rule(char in)
{
	std::string out;

	if      ( in == 'X' ) out = "F[+X]F[-X]+X";
	else if ( in == 'F' ) out = "FF";
	else                  out = in;

	return out;
}

std::string Reproduce(std::string input)
{
	std::string output;

	for ( unsigned int i=0; i<input.size(); i++ )
	{
		output = output + Rule( input[i] );
	}

	return output;
}

void CreateLSystemTree(int iteration, Mesh &out_mash)
{
	// Element 1
	Mesh trunk = glm::scale(glm::vec3(1, g_d, 1)) 
				* glm::translate(glm::vec3(0, 0.5f, 0)) 
				* g_cylinder_mesh;
	trunk.SetColor(1, 0, 1, 1);
	



	// String Reproduction
	std::string str = g_initial_str;

	for ( int i=0; i<iteration; i++ )
		str = Reproduce(str);


	std::stack<glm::mat4> stack_T;
	glm::mat4 T(1);
	for ( unsigned int i=0; i<str.size(); i++ )
	{
		if ( str[i] == 'F' )
		{
			out_mash += T*trunk;
			T = T * glm::translate(glm::vec3(0, g_d, 0));
		}
		else if ( str[i] == '+' )
		{
			glRotated(g_delta, 1, 0, 0);
			T = T * glm::rotate(g_delta, glm::vec3(1, 0, 0));
		}
		else if ( str[i] == '-' )
		{
			glRotated(-g_delta, 1, 0, 0);
			T = T * glm::rotate(-g_delta, glm::vec3(1, 0, 0));
		}
		else if ( str[i] == '[' )
		{
			stack_T.push(T);
		}
		else if ( str[i] == ']' )
		{
			T = stack_T.top();
			stack_T.pop();
		}
	}
}

