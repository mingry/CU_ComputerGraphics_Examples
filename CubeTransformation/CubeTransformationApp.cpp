


#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "InitShader.h"
#include "Camera.h"
#include "GroundObj.h"
#include "CubeObject.h"
#include "CubeTransformationApp.h"

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
	InitCube();

}







/**
ClearOpenGLResource: 프로그램이 끝나기 메모리 해제를 위해 한 번 호출되는 함수. (main 함수 참고)
프로그램에서 사용한 메모리를 여기에서 해제할 수 있다. 
예를들어, VAO와 VBO를 여기서 지울 수 있다.
*/
void ClearOpenGLResource()
{
	// Delete Mesh Models (VA0, VBO)
	DeleteCube();
	DeleteGround();
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
	//// 전체 화면을 지운다.
	// glClear는 Display 함수 가장 윗 부분에서 한 번만 호출되어야한다.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// 카메라 프로젝션 속성을 설정한다.
	int p_mat_loc = glGetUniformLocation(g_shader_id, "projection_matrix");
	glUniformMatrix4fv(p_mat_loc, 1, GL_FALSE, glm::value_ptr(g_camera.GetGLProjectionMatrix()));


	// 현재 카메라에 대한 변환 행렬을 받아온다.
	glm::mat4 camera_T = g_camera.GetGLViewMatrix();



	// 바닥 그리기.
	{
		// 변환(Transformation) 행렬 설정.
		glm::mat4 T = camera_T;

		// 변환 행렬을 Shader 프로그램에 있는 "modelview_matrix" 변수로 복사한다.
		int m_mat_loc = glGetUniformLocation(g_shader_id, "modelview_matrix");
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));
		
		// 그린다.
		DrawGround();
	}

	
	
	// Cube 그리기 1
	{
		// 변환(Transformation) 행렬 설정.
		glm::mat4 T = camera_T;

		// 변환 행렬을 Shader 프로그램에 있는 "modelview_matrix" 변수로 복사한다.
		int m_mat_loc = glGetUniformLocation(g_shader_id, "modelview_matrix");
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));
		
		// 그린다.
		DrawCube();
	}



	// Cube 그리기 2
	{
		// 변환(Transformation) 행렬 설정.
		
		// y축으로 200만큼 이동하는 행렬.
		glm::mat4 tran = glm::translate( glm::vec3(0.f, 200.f, 0.f) );

		// z축으로 45도 만큼 회전하는 행렬.
		glm::vec3 z_axis(0.f, 0.f, 1.f);
		glm::mat4 rot = glm::rotate( glm::pi<float>()/4.f, z_axis );



		// 모든 변환의 곱 (카메라 변환 행렬은 가장 마지막에 적용).
		glm::mat4 T = camera_T * rot * tran;



		// 변환 행렬을 Shader 프로그램에 있는 "modelview_matrix" 변수로 복사한다.
		int m_mat_loc = glGetUniformLocation(g_shader_id, "modelview_matrix");
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));
		

		// 그린다.
		DrawCube();
	}


	// Cube 그리기 3
	{
		// 변환(Transformation) 행렬 설정.
		
		// 2배 축소하는 행렬.
		glm::mat4 sca = glm::scale( glm::vec3(0.5f, 0.5f, 0.5f) );

		// y축으로 30도 만큼 회전하는 행렬.
		glm::vec3 y_axis(0.f, 1.f, 0.f);
		glm::mat4 rot = glm::rotate( glm::pi<float>()/6.f, y_axis );

		// x축으로 200만큼 이동하는 행렬.
		glm::mat4 tran = glm::translate( glm::vec3(200.f, 0.f, 0.f) );

		

		// 모든 변환의 곱 (카메라 변환 행렬은 가장 마지막에 적용).
		glm::mat4 T = camera_T * tran * rot * sca;



		// 변환 행렬을 Shader 프로그램에 있는 "modelview_matrix" 변수로 복사한다.
		int m_mat_loc = glGetUniformLocation(g_shader_id, "modelview_matrix");
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(T));
		

		// 그린다.
		DrawCube();
	}
	


	// 더블버퍼링 수행을 위한 함수 호출.
	// glutSwapBuffers는 항상 Display 함수 가장 아래 부분에서 한 번만 호출되어야한다.
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



	// glutPostRedisplay는 가능한 빠른 시간 안에 전체 그림을 다시 그릴 것을 시스템에 요청한다.
	// 결과적으로 Display() 함수를 호출하게 된다.
	glutPostRedisplay();
} 








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

	// 아래는 카메라 위치와 방향을 조정하기 위한 코드이다.
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

	// 아래는 카메라 위치와 방향을 조정하기 위한 코드이다.
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


