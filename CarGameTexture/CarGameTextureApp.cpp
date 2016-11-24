
#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "InitShader.h"
#include "Camera.h"
#include "GroundTexture.h"
#include "BasicShapeObjs.h"
#include "CarModelTexture.h"




// Camera 
static Camera g_camera;


// Window and User Interface
static bool g_left_button_pushed;
static bool g_right_button_pushed;
static int g_last_mouse_x;
static int g_last_mouse_y;
static int g_window_w;
static int g_window_h;



// Shader Program ID
int g_shader_id;


// Animtion Timer
// �ִϸ��̼� �ð��� ���õ� ������.
static const float FPS = 60.f;
void Timer(int value);
static float g_elapsed_time = 0.0f;  // ���α׷� ���� �������� �귯�� �ð� (�� ����).




// Car Position, Rotation, Velocity
// �ڵ��� ���� ������.
float g_car_position_x = 0;		// ��ġ
float g_car_position_z = 0;		// ��ġ
float g_car_speed = 0;			// �ӵ�
float g_car_rotation_y = 0;		// ���� ���� (y�� ȸ��)
float g_car_angular_speed = 0;	// ȸ�� �ӵ� (���ӵ�)



/**
InitOpenGL: ���α׷� �ʱ� �� ������ ���� ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
OpenGL�� ������ �ʱ� ���� ���α׷��� �ʿ��� �ٸ� �ʱ� ���� �����Ѵ�.
�������, VAO�� VBO�� ���⼭ ������ �� �ִ�.
*/
void InitOpenGL()
{
	// Vertex Shader�� Fragment Shader �ڵ尡 �ִ� ������ �����ϰ� ����� �� �ֵ��� �غ��Ѵ�.
	g_shader_id = CreateFromFiles("Shader/vshader_Texture.glsl", "Shader/fshader_Texture.glsl");
	glUseProgram(g_shader_id);
	
	// ������ ���Ѵ�.
	glClearColor(1, 1, 1, 0);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	// Initial State of Camera
	// �ʱ� ī�޶� �Ӽ��� �����Ѵ�.
	g_camera.setFov(45.0f);
	g_camera.setNearFar(0.1, 10000);
	g_camera.setRotation(glm::quat(cos(glm::pi<float>()/2), 0, sin(glm::pi<float>()/2), 0));
	g_camera.setTranslation(glm::vec3(0, 110, 590));

	// Initialize Mesh Models (VAO, VBO)
	// �ʿ��� VAO, VBO�� �����Ѵ�.
	InitGroundTexture();
	InitBasicShapeObjs();
	InitCarModel();

	// 1/60�� �Ŀ� Timer �Լ��� ȣ��ǵ��� ���� �Ѵ�.
	glutTimerFunc((unsigned int)(1000/FPS), Timer, 0);
}



/**
ClearOpenGLResource: ���α׷��� ������ �޸� ������ ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
���α׷����� ����� �޸𸮸� ���⿡�� ������ �� �ִ�. 
�������, VAO�� VBO�� ���⼭ ���� �� �ִ�.
*/
void ClearOpenGLResource()
{
	// Delete Mesh Models (VA0, VBO)
	DeleteBasicShapeObjs();
	DeleteCarModel();
	DeleteGroundTexture();
}



/**
Timer: �ִϸ��̼��� ���� �ֱ������� ȣ��Ǵ� �Լ�.
�� ���α׷������� 1�ʿ� 60�� ȣ��ǵ��� �����Ѵ�.
�귯�� �ð��� ������Ʈ�ϰ� �̸� �������� ��ǳ�� ������ ������ ���� ����Ѵ�.
*/
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






/**
Display: �׸��� �׸��� ���� ȣ��Ǵ� callback �Լ�. 
ȭ�鿡 �׸��� ���� �׷��� �ʿ䰡 ���� ������ �ڵ����� �ݺ��ؼ� ���� �� ȣ��ȴ�.
(��. �ٸ� �����쿡 ���� ȭ���� �Ϻκ��� �������ٰ� �ٽ� ��Ÿ���� ���.)
�ݺ������� ȣ��ȴٴ� ���� ����ϰ�, ���ʿ��� ������ ���� �̰��� �־�� �ȵȴ�.
���� ���, VAO�� VBO�� �����ϴ� �ڵ带 �̰��� �־�� �ȵȴ�. �� ������ �̹� ������
VAO�� VBO�� ����ؼ� �׸��� �׸��� �ڵ常 ���ԵǾ�� �Ѵ�.
*/
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
		glm::mat4 modelview_T = view_m;
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(modelview_T));
		DrawGroundTexture();
	}


	// Moving Car
	{
		glm::mat4 modelview_T = view_m * car_m  * glm::scale(glm::vec3(0.5, 0.5, 0.5));
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(modelview_T));
		DrawCarModel();
	}


	

	glutSwapBuffers();
}




/**
Reshape: �������� ũ�Ⱑ ������ ������ �ڵ����� ȣ��Ǵ� callback �Լ�. 
w�� h�� ���� ������ �������� ���� ũ��� ���� ũ�⸦ ��Ÿ���� (�ȼ� ����).
ȭ�� ũ�Ⱑ ��ȭ�ϸ�, Viewport�� ũ��, ī�޶��� ���μ��� ���� ���� �ٽ� �����ؾ��Ѵ�.
*/
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



/**
Mouse: ���콺 ��ư�� �Էµ� ������ �ڵ����� ȣ��Ǵ� �Լ�. 
�Ķ������ �ǹ̴� ������ ����.
1. button: ���� ��ư�� ���� 
  GLUT_LEFT_BUTTON - ���� ��ư
  GLUT_RIGHT_BUTTON - ������ ��ư 
  GLUT_MIDDLE_BUTTON - ��� ��ư (���� �������� ��) 
  3 - ���콺 �� (���� ���� ���� ����).
  4 - ���콺 �� (���� �Ʒ��� ���� ����).
2. state: ���� ���� 
  GLUT_DOWN - ���� ����
  GLUT_UP - ��������
3. x,y: ������ �Ͼ�� ��, ���콺 �������� ��ǥ��.
*/
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
		g_camera.inputMouse(Camera::IN_TRANS_Z, 0, -1);
		glutPostRedisplay();
	}
	else if ( button == 4 )
	{
		g_camera.inputMouse(Camera::IN_TRANS_Z, 0, 1);
		glutPostRedisplay();
	}

	g_last_mouse_x = x;
	g_last_mouse_y = y;
}





/**
MouseMotion: ���콺 �����Ͱ� ������ ������ �ڵ����� ȣ��Ǵ� �Լ�. 
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ���� ��Ÿ����.
*/
void MouseMotion(int x, int y)
{
	double mouse_xd = (double)x / g_window_w;
	double mouse_yd = 1 - (double)y / g_window_h;
	double last_mouse_xd = (double)g_last_mouse_x / g_window_w;
	double last_mouse_yd = 1 - (double)g_last_mouse_y / g_window_h;

	if ( g_left_button_pushed )
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



/**
Keyboard: Ű���� �Է��� ���� ������ �ڵ����� ȣ��Ǵ� �Լ�. 
�Ķ���� key �� ������ Ű������ ���ڰ��� ��Ÿ����.
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ���� ��Ÿ����.
*/
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




/**
SpeicalKeyboard: ���ڰ����� ǥ���ϱ� ����� Ű���� Ű�� ������ ������ �ڵ����� ȣ��Ǵ� �Լ�. 
�Ķ���� key �� ������ Ű���带 ��Ÿ���� ��ũ�� �� (freeglut_std.h ����).
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ��.
*/
void SpeicalKeyboard(int key, int x, int y )
{
	switch (key)						
	{
	case GLUT_KEY_DOWN:
		std::cout << "down-key pushed" << std::endl;
		g_car_speed = -1.0;		// ���� �ӵ� ����
		break;

	case GLUT_KEY_UP:
		std::cout << "up-key pushed" << std::endl;
		g_car_speed = 1.0;		// ���� �ӵ� ����
		break;

	case GLUT_KEY_LEFT:
		std::cout << "left-key pushed" << std::endl;
		g_car_angular_speed = 1.0/180.0 * glm::pi<float>();		// ��ȸ�� ���ӵ� ����
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "right-key pushed" << std::endl;
		g_car_angular_speed = -1.0/180.0 * glm::pi<float>();		//  ��ȸ�� ���ӵ� ����
		break;

	}

	glutPostRedisplay();
	return;
}




/**
SpeicalKeyboardUp: ���ڰ����� ǥ���ϱ� ����� Ű���� Ű�� (�������ٰ�) ������ ������ �ڵ����� ȣ��Ǵ� �Լ�. 
�Ķ���� key �� ������ Ű���带 ��Ÿ���� ��ũ�� �� (freeglut_std.h ����).
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ��.
*/
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