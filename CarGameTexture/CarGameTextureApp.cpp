
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
// 애니메이션 시간에 관련된 변수들.
static const float FPS = 60.f;
void Timer(int value);
static float g_elapsed_time = 0.0f;  // 프로그램 시작 시점부터 흘러간 시간 (초 단위).




// Car Position, Rotation, Velocity
// 자동차 제어 변수들.
float g_car_position_x = 0;		// 위치
float g_car_position_z = 0;		// 위치
float g_car_speed = 0;			// 속도
float g_car_rotation_y = 0;		// 현재 방향 (y축 회전)
float g_car_angular_speed = 0;	// 회전 속도 (각속도)



/**
InitOpenGL: 프로그램 초기 값 설정을 위해 최초 한 번 호출되는 함수. (main 함수 참고)
OpenGL에 관련한 초기 값과 프로그램에 필요한 다른 초기 값을 설정한다.
예를들어, VAO와 VBO를 여기서 생성할 수 있다.
*/
void InitOpenGL()
{
	// Vertex Shader와 Fragment Shader 코드가 있는 파일을 지정하고 사용할 수 있도록 준비한다.
	g_shader_id = CreateFromFiles("Shader/vshader_Texture.glsl", "Shader/fshader_Texture.glsl");
	glUseProgram(g_shader_id);
	
	// 배경색을 정한다.
	glClearColor(1, 1, 1, 0);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	// Initial State of Camera
	// 초기 카메라 속성을 설정한다.
	g_camera.setFov(45.0f);
	g_camera.setNearFar(0.1, 10000);
	g_camera.setRotation(glm::quat(cos(glm::pi<float>()/2), 0, sin(glm::pi<float>()/2), 0));
	g_camera.setTranslation(glm::vec3(0, 110, 590));

	// Initialize Mesh Models (VAO, VBO)
	// 필요한 VAO, VBO를 생성한다.
	InitGroundTexture();
	InitBasicShapeObjs();
	InitCarModel();

	// 1/60초 후에 Timer 함수가 호출되도록 설정 한다.
	glutTimerFunc((unsigned int)(1000/FPS), Timer, 0);
}



/**
ClearOpenGLResource: 프로그램이 끝나기 메모리 해제를 위해 한 번 호출되는 함수. (main 함수 참고)
프로그램에서 사용한 메모리를 여기에서 해제할 수 있다. 
예를들어, VAO와 VBO를 여기서 지울 수 있다.
*/
void ClearOpenGLResource()
{
	// Delete Mesh Models (VA0, VBO)
	DeleteBasicShapeObjs();
	DeleteCarModel();
	DeleteGroundTexture();
}



/**
Timer: 애니메이션을 위해 주기적으로 호출되는 함수.
이 프로그램에서는 1초에 60번 호출되도록 설계한다.
흘러간 시간을 업데이트하고 이를 바탕으로 선풍기 모터의 각도를 새로 계산한다.
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
Display: 그림을 그리기 위해 호출되는 callback 함수. 
화면에 그림이 새로 그려질 필요가 있을 때마다 자동으로 반복해서 여러 번 호출된다.
(예. 다른 윈도우에 의해 화면의 일부분이 가려졌다가 다시 나타나는 경우.)
반복적으로 호출된다는 것을 명심하고, 불필요한 내용은 절대 이곳에 넣어서는 안된다.
예를 들어, VAO나 VBO를 생성하는 코드를 이곳에 넣어서는 안된다. 이 곳에는 이미 생성된
VAO와 VBO를 사용해서 그림을 그리는 코드만 포함되어야 한다.
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
Reshape: 윈도우의 크기가 조정될 때마다 자동으로 호출되는 callback 함수. 
w와 h는 각각 조정된 윈도우의 가로 크기와 세로 크기를 나타낸다 (픽셀 단위).
화면 크기가 변화하면, Viewport의 크기, 카메라의 가로세로 비율 등을 다시 설정해야한다.
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
Mouse: 마우스 버튼이 입력될 때마다 자동으로 호출되는 함수. 
파라메터의 의미는 다음과 같다.
1. button: 사용된 버튼의 종류 
  GLUT_LEFT_BUTTON - 왼쪽 버튼
  GLUT_RIGHT_BUTTON - 오른쪽 버튼 
  GLUT_MIDDLE_BUTTON - 가운데 버튼 (휠이 눌러졌을 때) 
  3 - 마우스 휠 (휠이 위로 돌아 갔음).
  4 - 마우스 휠 (휠이 아래로 돌아 갔음).
2. state: 조작 상태 
  GLUT_DOWN - 눌러 졌음
  GLUT_UP - 놓여졌음
3. x,y: 조작이 일어났을 때, 마우스 포인터의 좌표값.
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
MouseMotion: 마우스 포인터가 움직일 때마다 자동으로 호출되는 함수. 
파라메터 x,y는 현재 마우스 포인터의 좌표값을 나타낸다.
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
Keyboard: 키보드 입력이 있을 때마다 자동으로 호출되는 함수. 
파라메터 key 는 눌려진 키보드의 문자값을 나타낸다.
파라메터 x,y는 현재 마우스 포인터의 좌표값을 나타낸다.
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
SpeicalKeyboard: 문자값으로 표현하기 어려운 키보드 키가 눌러질 때마다 자동으로 호출되는 함수. 
파라메터 key 는 눌려진 키보드를 나타내는 매크로 값 (freeglut_std.h 참고).
파라메터 x,y는 현재 마우스 포인터의 좌표값.
*/
void SpeicalKeyboard(int key, int x, int y )
{
	switch (key)						
	{
	case GLUT_KEY_DOWN:
		std::cout << "down-key pushed" << std::endl;
		g_car_speed = -1.0;		// 후진 속도 설정
		break;

	case GLUT_KEY_UP:
		std::cout << "up-key pushed" << std::endl;
		g_car_speed = 1.0;		// 전진 속도 설정
		break;

	case GLUT_KEY_LEFT:
		std::cout << "left-key pushed" << std::endl;
		g_car_angular_speed = 1.0/180.0 * glm::pi<float>();		// 좌회전 각속도 설정
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "right-key pushed" << std::endl;
		g_car_angular_speed = -1.0/180.0 * glm::pi<float>();		//  우회전 각속도 설정
		break;

	}

	glutPostRedisplay();
	return;
}




/**
SpeicalKeyboardUp: 문자값으로 표현하기 어려운 키보드 키가 (눌러졌다가) 놓여질 때마다 자동으로 호출되는 함수. 
파라메터 key 는 눌려진 키보드를 나타내는 매크로 값 (freeglut_std.h 참고).
파라메터 x,y는 현재 마우스 포인터의 좌표값.
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