
#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "InitShader.h"
#include "Camera.h"
#include "GroundObj2.h"
#include "BasicShapeObjs.h"
#include "CarModel.h"




// Camera 
static Camera g_camera;


// Window and User Interface
static bool g_left_button_pushed;
static bool g_right_button_pushed;
static int g_last_mouse_x;
static int g_last_mouse_y;
static int g_window_w;
static int g_window_h;


void InitWindow();
void Reshape( int w, int h );


// Keyboard and Mouse
void Keyboard(unsigned char key, int x, int y );
void SpeicalKeyboard(int key, int x, int y );
void SpeicalKeyboardUp(int key, int x, int y );
void Mouse(int button, int state, int x, int y);
void MouseMotion(int x, int y);




// Car Position, Rotation, Velocity
float g_car_position_x = 0;
float g_car_position_z = 0;
float g_car_speed = 0;
float g_car_rotation_y = 0;
float g_car_angular_speed = 0;


// OpenGL
void InitOpenGL();
void Display();


// Shader Program ID
int g_shader_id;


// Animtion Timer
static const float FPS = 60.f;
void Timer(int value);
static float g_elapsed_time = 0.0f;  // sec




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

	InitGround2();
	InitBasicShapeObjs();
	InitCarModel();

	
	glutReshapeFunc( Reshape );
	glutDisplayFunc( Display );
	glutKeyboardFunc( Keyboard );
	glutSpecialFunc( SpeicalKeyboard );
	glutSpecialUpFunc( SpeicalKeyboardUp );
	glutMouseFunc( Mouse );
	glutMotionFunc( MouseMotion );
	glutTimerFunc((unsigned int)(1000.f/FPS), Timer, 0);
	
	glutMainLoop();

	DeleteGround2();
	DeleteBasicShapeObjs();
	DeleteCarModel();

	return 0;
}



void InitOpenGL()
{
	g_shader_id = CreateFromFiles("Shader/vshader_toon.glsl", "Shader/fshader_toon.glsl");
	glUseProgram(g_shader_id);
	
	glClearColor(1, 1, 1, 1);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	// Initial State of Camera
	g_camera.setFov(45.0f);
	g_camera.setNearFar(0.1, 10000);
	g_camera.setRotation(glm::quat(cos(glm::pi<float>()/2), 0, sin(glm::pi<float>()/2), 0));
	g_camera.setTranslation(glm::vec3(0, 110, 590));

}

void Timer(int value)
{
	g_elapsed_time += 1.0f/FPS;

	// Turn
	g_car_rotation_y += g_car_angular_speed;
	
	// Calculate Velocity
	double velocity_x = g_car_speed * sin(g_car_rotation_y);
	double velocity_z = g_car_speed * cos(g_car_rotation_y);

	// Move
	g_car_position_x += velocity_x;
	g_car_position_z += velocity_z;


	glutPostRedisplay();
	glutTimerFunc((unsigned int)(1000.f/FPS), Timer, 0);
}





void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 projection_m = g_camera.GetGLProjectionMatrix();  // Mouse-Controlling Camera Projection
	glm::mat4 view_m       = g_camera.GetGLViewMatrix();        // Mouse-Controlling Camera View Matrix
	
	glm::mat4 car_m;
	car_m = glm::translate(glm::vec3(g_car_position_x, 0, g_car_position_z)) 
		  * glm::rotate(g_car_rotation_y, glm::vec3(0, 1, 0));
	
	
	int p_mat_loc = glGetUniformLocation(g_shader_id, "projection_matrix");
	int m_mat_loc = glGetUniformLocation(g_shader_id, "modelview_matrix");
	
	
	glUniformMatrix4fv(p_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_m));


	// Ground
	{
		glUniform1i(glGetUniformLocation(g_shader_id, "shading_mode"), 0);
		glm::mat4 modelview_T = view_m;
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(modelview_T));
		DrawGround2();
	}


	// Moving Car
	{
		glUniform1i(glGetUniformLocation(g_shader_id, "shading_mode"), 1);
		glm::mat4 modelview_T = view_m * car_m;
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(modelview_T));
		DrawCarModel();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::vec3 light_direction(1.f, 1.f, 1.f);
		glUniform1i(glGetUniformLocation(g_shader_id, "shading_mode"), 2);
		glm::mat4 shadow_m = glm::mat4(glm::vec4(1,0,0,0), 
					glm::vec4(-(light_direction.x/light_direction.z) ,0, -(light_direction.y/light_direction.z), 0), 
					glm::vec4(0,0,1,0), 
					glm::vec4(0,1,0,1));

		modelview_T = view_m * shadow_m * car_m;
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(modelview_T));
		DrawCarModel();

		glDisable(GL_BLEND);

	}


	

	glutSwapBuffers();
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

void Reshape( int w, int h )
{     
	//  w : window width   h : window height
	g_window_w = w;
	g_window_h = h;
	g_left_button_pushed = false;
	g_right_button_pushed = false;

	
	//  w : window width   h : window height
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	g_camera.setAspectRatio((double)w/(double)h);
	
} 









/////////////////////////////////////////////////////////////////////////////////////////
/// Keyboard and Mouse Input

void Mouse(int button, int state, int x, int y)
{
	double mouse_xd = (double)x / g_window_w;
	double mouse_yd = 1 - (double)y / g_window_h;
	double last_mouse_xd = (double)g_last_mouse_x / g_window_w;
	double last_mouse_yd = 1 - (double)g_last_mouse_y / g_window_h;


	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		g_left_button_pushed = true;

	else if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		g_left_button_pushed = false;

	else if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		g_right_button_pushed = true;

	else if ( button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		g_right_button_pushed = false;
	else if ( button == 3 )
	{
		g_camera.inputMouse(Camera::IN_ZOOM, 0, 1);
		glutPostRedisplay();
	}
	else if ( button == 4 )
	{
		g_camera.inputMouse(Camera::IN_ZOOM, 0, -1);
		glutPostRedisplay();
	}

	g_last_mouse_x = x;
	g_last_mouse_y = y;
}

void MouseMotion(int x, int y)
{
	double mouse_xd = (double)x / g_window_w;
	double mouse_yd = 1 - (double)y / g_window_h;
	double last_mouse_xd = (double)g_last_mouse_x / g_window_w;
	double last_mouse_yd = 1 - (double)g_last_mouse_y / g_window_h;

	if ( g_left_button_pushed && g_right_button_pushed )
	{
		g_camera.inputMouse(Camera::IN_ZOOM, last_mouse_xd, last_mouse_yd, mouse_xd, mouse_yd);
		glutPostRedisplay();
	}
	else if ( g_left_button_pushed )
	{
		g_camera.inputMouse(Camera::IN_ROTATION_Y_UP, last_mouse_xd, last_mouse_yd, mouse_xd, mouse_yd);
		glutPostRedisplay();
	}
	else if ( g_right_button_pushed )
	{
		g_camera.inputMouse(Camera::IN_TRANS, last_mouse_xd, last_mouse_yd, mouse_xd, mouse_yd);
		glutPostRedisplay();
	}

	g_last_mouse_x = x;
	g_last_mouse_y = y;
}


void Keyboard(unsigned char key, int x, int y )
{
	switch (key)						
	{
	case '1':
		break;

	case '2':
		break;

	case '3':
		break;

	case '4':
		break;

	case '5':
		break;

	case 27 : exit(0); break;			
	default : 
		std::cout << "key: " << key << std::endl;
		break;
	}

	glutPostRedisplay();
	return;
}



void SpeicalKeyboard(int key, int x, int y )
{
	switch (key)						
	{
	case GLUT_KEY_DOWN:
		std::cout << "down-key pushed" << std::endl;
		g_car_speed = -1.0;
		break;

	case GLUT_KEY_UP:
		std::cout << "up-key pushed" << std::endl;
		g_car_speed = 1.0;
		break;

	case GLUT_KEY_LEFT:
		std::cout << "left-key pushed" << std::endl;
		g_car_angular_speed = 1.0/180.0 * glm::pi<float>();
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "right-key pushed" << std::endl;
		g_car_angular_speed = -1.0/180.0 * glm::pi<float>();
		break;

	}

	glutPostRedisplay();
	return;
}

void SpeicalKeyboardUp(int key, int x, int y )
{
	switch (key)						
	{
	case GLUT_KEY_DOWN:
		std::cout << "down-key released" << std::endl;
		g_car_speed = 0.0;
		break;

	case GLUT_KEY_UP:
		std::cout << "up-key released" << std::endl;
		g_car_speed = 0.0;
		break;

	case GLUT_KEY_LEFT:
		std::cout << "left-key released" << std::endl;
		g_car_angular_speed = 0.0;
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "right-key released" << std::endl;
		g_car_angular_speed = 0.0;
		break;

	}

	glutPostRedisplay();
	return;
}











