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
// 애니메이션 시간에 관련된 변수들.
static const float FPS = 60.f;
void Timer(int value);
static float g_elapsed_time = 0.0f;  // 프로그램 시작 시점부터 흘러간 시간 (초 단위).
static float g_motor_angle = 0.0f;	// 선풍기 모터의 현재 회전각을 저장.


// Draw Mode
int g_draw_mode = 1;






/**
InitOpenGL: 프로그램 초기 값 설정을 위해 최초 한 번 호출되는 함수. (main 함수 참고)
OpenGL에 관련한 초기 값과 프로그램에 필요한 다른 초기 값을 설정한다.
예를들어, VAO와 VBO를 여기서 생성할 수 있다.
*/
void InitOpenGL()
{
	// Vertex Shader와 Fragment Shader 코드가 있는 파일을 지정하고 사용할 수 있도록 준비한다.
	g_shader_id = CreateFromFiles("Shader/vshader.glsl", "Shader/fshader.glsl");
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
	InitGround();
	InitFanObjects();


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
	DeleteGround();
	DeleteFanObjects();
}




/**
Timer: 애니메이션을 위해 주기적으로 호출되는 함수.
이 프로그램에서는 1초에 60번 호출되도록 설계한다.
흘러간 시간을 업데이트하고 이를 바탕으로 선풍기 모터의 각도를 새로 계산한다.
*/
void Timer(int value)
{
	g_elapsed_time += 1.0f/FPS;

	g_motor_angle = 2.f * glm::pi<float>() * g_elapsed_time;


	// glutPostRedisplay는 가능한 빠른 시간 안에 전체 그림을 다시 그릴 것을 시스템에 요청한다.
	// 결과적으로 Display() 함수를 호출하게 된다.
	glutPostRedisplay();

	// 1/60 초 후에 Timer 함수가 다시 호출되로록 한다.
	glutTimerFunc((unsigned int)(1000/FPS), Timer, 0);
}



void Display1();
void Display2();
void Display3();
void Display4();


/**
Display: 그림을 그리기 위해 호출되는 callback 함수. 
화면에 그림이 새로 그려질 필요가 있을 때마다 자동으로 반복해서 여러 번 호출된다.
(예. 다른 윈도우에 의해 화면의 일부분이 가려졌다가 다시 나타나는 경우.)
반복적으로 호출된다는 것을 명심하고, 불필요한 내용은 절대 이곳에 넣어서는 안된다.
예를 들어, VAO나 VBO를 생성하는 코드를 이곳에 넣어서는 안된다. 이 곳에는 이미 생성된
VAO와 VBO를 사용해서 그림을 그리는 코드만 포함되어야 한다.

이 예제의 경우에는 g_draw_mode가 변함에 따라 다른 그림이 나타나도록 하였다.
*/
void Display()
{
	// 전체 화면을 지운다.
	// glClear는 Display 함수 가장 윗 부분에서 한 번만 호출되어야한다.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	// 카메라 프로젝션 속성을 설정한다.
	int p_mat_loc = glGetUniformLocation(g_shader_id, "projection_matrix");
	glUniformMatrix4fv(p_mat_loc, 1, GL_FALSE, glm::value_ptr(g_camera.GetGLProjectionMatrix()));

	// 카메라 위치와 앞으로 그려질 모델의 위치를 설정한다.
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


	// 더블버퍼링 수행을 위한 함수 호출.
	// glutSwapBuffers는 항상 Display 함수 가장 아래 부분에서 한 번만 호출되어야한다.
	glutSwapBuffers();
}




/**
Displa1y: 분해되어있는 선풍기를 그리는 함수. 
*/
void Display1()
{
	// Get modelview metrix Location
	// Shader 프로그램 에서 Model-view 변환 행렬을 저장할 위치를 받아온다. (vshader.glsl 파일 참조)
	int m_mat_loc = glGetUniformLocation(g_shader_id, "modelview_matrix");


	// Get Camera Transfomration
	// 현재 카메라 변환 행렬을 받아온다.
	glm::mat4 camera_T = g_camera.GetGLViewMatrix();




	// Base
	// 선풍기 베이스 부분.
	{
		// 1. 변환 행렬 값을 계산한다.
		glm::mat4 T = camera_T;
		glm::mat4 trans = glm::translate( glm::vec3(-120.f, 0.f, 0.f)  );
		
		// 1.1. 카메라 변환 행결과 베이스 부분 변환 행렬을 곱한다.
		T = T * trans;
		
		// 2. 변환 행렬 값을 Shader 프로그램으로 보낸다.
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));

		// 3. 그린다.
		// 위에서 설정된 변환 행렬이 반영된다.(vshader.glsl 파일 참조)
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
MouseMotion: 마우스 포인터가 움직일 때마다 자동으로 호출되는 함수. 
파라메터 x,y는 현재 마우스 포인터의 좌표값을 나타낸다.
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
Keyboard: 키보드 입력이 있을 때마다 자동으로 호출되는 함수. 
파라메터 key 는 눌려진 키보드의 문자값을 나타낸다.
파라메터 x,y는 현재 마우스 포인터의 좌표값을 나타낸다.

이 예제에서는 키보드 '1', '2', '3', '4' 가 눌려지면 g_draw_mode를 변경한다.
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


	// glutPostRedisplay는 가능한 빠른 시간 안에 전체 그림을 다시 그릴 것을 시스템에 요청한다.
	// 결과적으로 Display() 함수를 호출하게 된다.
	glutPostRedisplay();
	return;
}




/**
SpeicalKeyboard: 문자값으로 표현하기 어려운 키보드 입력이 있을 때마다 자동으로 호출되는 함수. 
파라메터 key 는 눌려진 키보드를 나타내는 매크로 값 (freeglut_std.h 참고).
파라메터 x,y는 현재 마우스 포인터의 좌표값.
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
