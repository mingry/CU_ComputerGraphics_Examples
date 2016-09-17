#include <iostream>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <math.h>



int main( int argc, char** argv )
{

	std::cout << "* GLM Vector"  << std::endl;

	// Initialize v0 and v1
	glm::vec3 v0(1, 2, -3);
	
	glm::vec3 v1;
	v1.x = -4;
	v1.y = 5;
	v1.z = 6;

	// Vector Addition
	glm::vec3 v2 = v0 + v1;

	// Scalar Multiplication
	glm::vec3 v3 = 2.5f * v1;

	// Normalize (make a unit vector)
	glm::vec3 unit_v1 = glm::normalize(v1);

	// Length
	float length_v1 = glm::length(v1);

	// Dot Product
	float dot_v0_v1 = glm::dot(v0, v1);
	
	// Cross Product
	glm::vec3 cross_v0_v1 = glm::cross(v0, v1);
	

	
	// Print out
	std::cout << "v0 = " << glm::to_string(v0) << std::endl << std::endl;
	std::cout << "v1 = " << glm::to_string(v1) << std::endl << std::endl;
	std::cout << "v0 + v1 = " << glm::to_string(v2) << std::endl << std::endl;
	std::cout << "2.5 * v1 = " << glm::to_string(v3) << std::endl << std::endl;
	std::cout << "Length of v1 = " << length_v1 << std::endl << std::endl;
	std::cout << "Dot product of v0 and v1 = " << dot_v0_v1 << std::endl << std::endl;
	std::cout << "Cross product of v0 and v1 = " << glm::to_string(cross_v0_v1) << std::endl << std::endl;


	


	std::cout << std::endl << std::endl << "* GLM Matrix"  << std::endl;


	// 3x3 identity matrix
	glm::mat3 m0(1.0f);

	// 3x3 matrix
	glm::mat3 m1(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 0.0f, 9.0f);

	
	// Assign and Change an element
	glm::mat3 m2 = m1;
	m2[1][2] = -5.0f;


	// Matrix Inverse
	glm::mat3 m1_inverse = glm::inverse(m1);

	// Matrix Transpose
	glm::mat3 m1_transpose = glm::transpose(m1);

	// Matrix Multiplicaqtion
	glm::mat3 m1xm2 = m1 * m2;


	// Matrix * Vector
	glm::vec3 m1xv1 = m1 * v1;


	// Print out
	std::cout << "m0 = " << glm::to_string(m0) << std::endl << std::endl;
	std::cout << "m1 = " << glm::to_string(m1) << std::endl << std::endl;
	std::cout << "m2 = " << glm::to_string(m2) << std::endl << std::endl;
	std::cout << "Inverse of m1 = " << glm::to_string(m1_inverse) << std::endl << std::endl;
	std::cout << "Transpose of m1 = " << glm::to_string(m1_transpose) << std::endl << std::endl;
	std::cout << "m1 * m2 = " << glm::to_string(m1xm2) << std::endl << std::endl;
	std::cout << "m1 * v1 = " << glm::to_string(m1xv1) << std::endl << std::endl;



	return 0;
}




