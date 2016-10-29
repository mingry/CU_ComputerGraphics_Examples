#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "InitShader.h"
#include "Camera.h"
#include "GroundObj.h"
#include "FanApp.h"
#include "FanObjects.h"




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
static float g_motor_angle = 0.0f;	// ��ǳ�� ������ ���� ȸ������ ����.


// Draw Mode
int g_draw_mode = 1;






/**
InitOpenGL: ���α׷� �ʱ� �� ������ ���� ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
OpenGL�� ������ �ʱ� ���� ���α׷��� �ʿ��� �ٸ� �ʱ� ���� �����Ѵ�.
�������, VAO�� VBO�� ���⼭ ������ �� �ִ�.
*/
void InitOpenGL()
{
	// Vertex Shader�� Fragment Shader �ڵ尡 �ִ� ������ �����ϰ� ����� �� �ֵ��� �غ��Ѵ�.
	g_shader_id = CreateFromFiles("Shader/vshader.glsl", "Shader/fshader.glsl");
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
	InitGround();
	InitFanObjects();


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
	DeleteGround();
	DeleteFanObjects();
}




/**
Timer: �ִϸ��̼��� ���� �ֱ������� ȣ��Ǵ� �Լ�.
�� ���α׷������� 1�ʿ� 60�� ȣ��ǵ��� �����Ѵ�.
�귯�� �ð��� ������Ʈ�ϰ� �̸� �������� ��ǳ�� ������ ������ ���� ����Ѵ�.
*/
void Timer(int value)
{
	g_elapsed_time += 1.0f/FPS;

	g_motor_angle = 2.f * glm::pi<float>() * g_elapsed_time;


	// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
	// ��������� Display() �Լ��� ȣ���ϰ� �ȴ�.
	glutPostRedisplay();

	// 1/60 �� �Ŀ� Timer �Լ��� �ٽ� ȣ��Ƿη� �Ѵ�.
	glutTimerFunc((unsigned int)(1000/FPS), Timer, 0);
}



void Display1();
void Display2();
void Display3();
void Display4();


/**
Display: �׸��� �׸��� ���� ȣ��Ǵ� callback �Լ�. 
ȭ�鿡 �׸��� ���� �׷��� �ʿ䰡 ���� ������ �ڵ����� �ݺ��ؼ� ���� �� ȣ��ȴ�.
(��. �ٸ� �����쿡 ���� ȭ���� �Ϻκ��� �������ٰ� �ٽ� ��Ÿ���� ���.)
�ݺ������� ȣ��ȴٴ� ���� ����ϰ�, ���ʿ��� ������ ���� �̰��� �־�� �ȵȴ�.
���� ���, VAO�� VBO�� �����ϴ� �ڵ带 �̰��� �־�� �ȵȴ�. �� ������ �̹� ������
VAO�� VBO�� ����ؼ� �׸��� �׸��� �ڵ常 ���ԵǾ�� �Ѵ�.

�� ������ ��쿡�� g_draw_mode�� ���Կ� ���� �ٸ� �׸��� ��Ÿ������ �Ͽ���.
*/
void Display()
{
	// ��ü ȭ���� �����.
	// glClear�� Display �Լ� ���� �� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	// ī�޶� �������� �Ӽ��� �����Ѵ�.
	int p_mat_loc = glGetUniformLocation(g_shader_id, "projection_matrix");
	glUniformMatrix4fv(p_mat_loc, 1, GL_FALSE, glm::value_ptr(g_camera.GetGLProjectionMatrix()));

	// ī�޶� ��ġ�� ������ �׷��� ���� ��ġ�� �����Ѵ�.
	int m_mat_loc = glGetUniformLocation(g_shader_id, "modelview_matrix");
	glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(g_camera.GetGLViewMatrix()));


	DrawGround();


	if ( g_draw_mode == 1 )
	{
		// 1: Parted Fan Objects
		Display1();
	}

	else if ( g_draw_mode == 2 )
	{
		// 2: Assmbled Fan
		Display2();

	}

	else if ( g_draw_mode == 3 )
	{
		// 3: New Fan
		Display3();
	}
	
	else if ( g_draw_mode == 4 )
	{
		// 4: Many Fans
		Display4();
	}


	// ������۸� ������ ���� �Լ� ȣ��.
	// glutSwapBuffers�� �׻� Display �Լ� ���� �Ʒ� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
	glutSwapBuffers();
}




/**
Displa1y: ���صǾ��ִ� ��ǳ�⸦ �׸��� �Լ�. 
*/
void Display1()
{
	// Get modelview metrix Location
	// Shader ���α׷� ���� Model-view ��ȯ ����� ������ ��ġ�� �޾ƿ´�. (vshader.glsl ���� ����)
	int m_mat_loc = glGetUniformLocation(g_shader_id, "modelview_matrix");


	// Get Camera Transfomration
	// ���� ī�޶� ��ȯ ����� �޾ƿ´�.
	glm::mat4 camera_T = g_camera.GetGLViewMatrix();




	// Base
	// ��ǳ�� ���̽� �κ�.
	{
		// 1. ��ȯ ��� ���� ����Ѵ�.
		glm::mat4 T = camera_T;
		glm::mat4 trans = glm::translate( glm::vec3(-120.f, 0.f, 0.f)  );
		
		// 1.1. ī�޶� ��ȯ ���� ���̽� �κ� ��ȯ ����� ���Ѵ�.
		T = T * trans;
		
		// 2. ��ȯ ��� ���� Shader ���α׷����� ������.
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));

		// 3. �׸���.
		// ������ ������ ��ȯ ����� �ݿ��ȴ�.(vshader.glsl ���� ����)
		DrawBase();
	}


	// Neck
	{
		glm::mat4 T = camera_T;
		glm::mat4 trans = glm::translate( glm::vec3(-60.f, 0.f, 0.f)  );
		
		T = T * trans;
		
		// Set Transform
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));

		DrawNeck();
	}
	
	// Head and Motor
	{
		glm::mat4 T = camera_T;
		glm::mat4 trans = glm::translate( glm::vec3(0.f, 0.f, 0.f)  );
		
		T = T * trans;
		
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));
		DrawHead();
		

		glm::mat4 rot = glm::rotate(g_motor_angle, glm::vec3(0.f, 1.f, 0.f)  );
		T = T * rot;
		
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));

		DrawMotor();
	}

	// Wings
	{
		glm::mat4 T = camera_T;
		glm::mat4 trans = glm::translate( glm::vec3(60.f, 0.f, 0.f)  );
		
		T = T * trans;

		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));
		DrawWing(0);

		T = T * trans;
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));
		DrawWing(1);


		T = T * trans;
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));
		DrawWing(2);
	}




	
}

void Display2()
{
}

void Display3()
{
}

void Display4()
{
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



/**
Keyboard: Ű���� �Է��� ���� ������ �ڵ����� ȣ��Ǵ� �Լ�. 
�Ķ���� key �� ������ Ű������ ���ڰ��� ��Ÿ����.
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ���� ��Ÿ����.

�� ���������� Ű���� '1', '2', '3', '4' �� �������� g_draw_mode�� �����Ѵ�.
*/
void Keyboard(unsigned char key, int x, int y )
{
	switch (key)						
	{
	case '1':
		g_draw_mode = 1;
		break;

	case '2':
		g_draw_mode = 2;
		break;

	case '3':
		g_draw_mode = 3;
		break;

	case '4':
		g_draw_mode = 4;
		break;

	case 27 : exit(0); break;			
	default : 
		std::cout << "key: " << key << std::endl;
		break;
	}


	// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �׸� ���� �ý��ۿ� ��û�Ѵ�.
	// ��������� Display() �Լ��� ȣ���ϰ� �ȴ�.
	glutPostRedisplay();
	return;
}




/**
SpeicalKeyboard: ���ڰ����� ǥ���ϱ� ����� Ű���� �Է��� ���� ������ �ڵ����� ȣ��Ǵ� �Լ�. 
�Ķ���� key �� ������ Ű���带 ��Ÿ���� ��ũ�� �� (freeglut_std.h ����).
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ��.
*/
void SpeicalKeyboard(int key, int x, int y )
{
	switch (key)						
	{
		std::cout << key << std::endl;
		/* fill here */
	case GLUT_KEY_DOWN:
		std::cout << "key down" << std::endl;
		break;

	case GLUT_KEY_UP:
		std::cout << "key up" << std::endl;
		break;

	case GLUT_KEY_LEFT:
		std::cout << "key left" << std::endl;
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "key right" << std::endl;
		break;

	}

	glutPostRedisplay();
	return;
}
