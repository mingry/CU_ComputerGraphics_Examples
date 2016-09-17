


#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "InitShader.h"
#include "Camera.h"
#include "GroundObj.h"
#include "Objects.h"
#include "OpenGLApp.h"

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
	InitTwoTri();
	InitCube();

}







/**
ClearOpenGLResource: ���α׷��� ������ �޸� ������ ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
���α׷����� ����� �޸𸮸� ���⿡�� ������ �� �ִ�. 
�������, VAO�� VBO�� ���⼭ ���� �� �ִ�.
*/
void ClearOpenGLResource()
{
	// Delete Mesh Models (VA0, VBO)
	DeleteTwoTri();
	DeleteCube();
	DeleteGround();
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
	// ��ü ȭ���� �����.
	// glClear�� �׻� Display �Լ� ���� �� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// ī�޶� �������� �Ӽ��� �����Ѵ�.
	int p_mat_loc = glGetUniformLocation(g_shader_id, "projection_matrix");
	glUniformMatrix4fv(p_mat_loc, 1, GL_FALSE, glm::value_ptr(g_camera.GetGLProjectionMatrix()));

	// ī�޶� ��ġ�� ������ �׷��� ���� ��ġ�� �����Ѵ�.
	int m_mat_loc = glGetUniformLocation(g_shader_id, "modelview_matrix");
	glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(g_camera.GetGLViewMatrix()));
	

	// �׸��� �׸���.
	DrawGround();
	DrawTwoTri();
	//DrawCube();


	// ������۸� ������ ���� �Լ� ȣ��.
	// glutSwapBuffers�� �׻� Display �Լ� ���� �Ʒ� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
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



	// glutPostRedisplay�� ������ ���� �ð� �ȿ� ��ü �׸��� �ٽ� �⸱ ���� ��û�Ѵ�.
	// ��������� Display() �Լ��� ȣ���ϰ� �ȴ�.
	glutPostRedisplay();
} 








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

	// �Ʒ��� ī�޶� ��ġ�� ������ �����ϱ� ���� �ڵ��̴�.
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

	// �Ʒ��� ī�޶� ��ġ�� ������ �����ϱ� ���� �ڵ��̴�.
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


