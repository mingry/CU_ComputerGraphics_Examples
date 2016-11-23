#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "InitShader.h"
#include "CarGameTreeApp.h"





void InitWindow();




int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	InitWindow();          
	
	glewExperimental = TRUE;
	if (  glewInit() != GLEW_OK )
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}

	InitOpenGL();

	glutReshapeFunc( Reshape );
	glutDisplayFunc( Display );
	glutKeyboardFunc( Keyboard );
	glutSpecialFunc( SpeicalKeyboard );
	glutSpecialUpFunc( SpeicalKeyboardUp );
	glutMouseFunc( Mouse );
	glutMotionFunc( MouseMotion );
	
	glutMainLoop();

	ClearOpenGLResource();

	return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////
/// Window

void InitWindow()
{
	// specifies display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// specifies the size, in pixels, of your window
	glutInitWindowSize(1080 ,720);

	glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );


	// creates a window with an OpenGL context
	glutCreateWindow( "Computer Graphics" );
}
