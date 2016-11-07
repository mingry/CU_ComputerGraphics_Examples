
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
#include "MultiLightsExampleApp.h"


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


/**
InitOpenGL: ���α׷� �ʱ� �� ������ ���� ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
OpenGL�� ������ �ʱ� ���� ���α׷��� �ʿ��� �ٸ� �ʱ� ���� �����Ѵ�.
�������, VAO�� VBO�� ���⼭ ������ �� �ִ�.
*/
void InitOpenGL()
{
	// Vertex Shader�� Fragment Shader �ڵ尡 �ִ� ������ �����ϰ� ����� �� �ֵ��� �غ��Ѵ�.
	g_shader_id = CreateFromFiles("Shader/vshader_MultiLights.glsl", "Shader/fshader_MultiLights.glsl");
	glUseProgram(g_shader_id);
	
		// ������ ���Ѵ�.
	glClearColor(0.2, 0.2, 0.2, 1);
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
	InitGround2();
	InitBasicShapeObjs();


	// Timer �Լ� ȣ��.
	glutTimerFunc((unsigned int)(1000.f/FPS), Timer, 0);
	
}



/**
ClearOpenGLResource: ���α׷��� ������ �޸� ������ ���� �� �� ȣ��Ǵ� �Լ�. (main �Լ� ����)
���α׷����� ����� �޸𸮸� ���⿡�� ������ �� �ִ�. 
�������, VAO�� VBO�� ���⼭ ���� �� �ִ�.
*/
void ClearOpenGLResource()
{
	// Delete Mesh Models (VA0, VBO)
	DeleteGround2();
	DeleteBasicShapeObjs();
}




/**
Timer: �ִϸ��̼��� ���� �ֱ������� ȣ��Ǵ� �Լ�.
�� ���α׷������� 1�ʿ� 60�� ȣ��ǵ��� �����Ѵ�.
�귯�� �ð��� ������Ʈ�ϰ� �̸� �������� ��ǳ�� ������ ������ ���� ����Ѵ�.
*/
void Timer(int value)
{
	g_elapsed_time += 1.0f/FPS;


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
	// ��ü ȭ���� �����.
	// glClear�� Display �Լ� ���� �� �κп��� �� ���� ȣ��Ǿ���Ѵ�.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	// ī�޶� �������� �Ӽ��� �����Ѵ�.
	glm::mat4 projection_m = g_camera.GetGLProjectionMatrix();	// Camera Projection
	int p_mat_loc = glGetUniformLocation(g_shader_id, "projection_matrix");
	glUniformMatrix4fv(p_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_m));


	// ī�޶� ��ġ�� ������ �׷��� ���� ��ġ�� ������ ���� �ʿ��� ������ �޾ƿ´�.
	glm::mat4 view_m       = g_camera.GetGLViewMatrix();	// Camera View Matrix
	int m_mat_loc = glGetUniformLocation(g_shader_id, "modelview_matrix");



	// Lights, �� ����.
	{
		// ���� ������ Shader ���� num_of_lights �� �Ѱ��ش�.
		int num_of_lights_loc = glGetUniformLocation(g_shader_id, "num_of_lights");
		glUniform1i(num_of_lights_loc, 3);

		// Directional Light ����.
		{
			// ���� ���� ���� (0: Directionl Light, 1: Point Light, 2: Spot Light), fshader_MultiLights.glsl ����.
			int type_loc = glGetUniformLocation(g_shader_id, "lights[0].type");
			glUniform1i(type_loc, 0);



			// ���� ���� ����.
			glm::vec3 dir(-1.f, -1.f, 0.f);

			////// *** ���� ī�޶� ������ ����ϱ� ���� ī�޶� ��ȯ��� ����  ***
			//  dir�� framgment shader���� ó���Ǵ�. 
			//  fragment shader������ ī�޶� ��ȯ�� �����ϴ� �κ��� ����.
			//  ���� �̸� ī�޶� ��ȯ ����� ���ؼ� �Ѱ��ش�.
			//  �̶� dir�� ������ ��Ÿ���� �����̹Ƿ� �̵�(Translation)��ȯ�� ���õǵ��� �Ѵ�.
			dir = glm::vec3(  view_m * glm::vec4(dir, 0.f) );

			int dir_loc = glGetUniformLocation(g_shader_id, "lights[0].dir");
			glUniform3f(dir_loc, dir[0], dir[1], dir[2]);

			// ���� ���� ����.
			int intensity_loc = glGetUniformLocation(g_shader_id, "lights[0].intensity");
			glUniform3f(intensity_loc, 0.3f, 0.3f, 0.3f);
		}


		// Point Light ����
		{
			// ���� ���� ���� (0: Directionl Light, 1: Point Light, 2: Spot Light), fshader_MultiLights.glsl ����.
			int type_loc = glGetUniformLocation(g_shader_id, "lights[1].type");
			glUniform1i(type_loc, 1);


			// ���� ����ϴ� ��ġ(����) ����.
			// �ð��� �帧�� ���� ��ġ�� ���ϵ��� ��.
			glm::vec3 pos(200.f * cos(g_elapsed_time), 50.f, 200.f* sin(g_elapsed_time));

			// Apply Camera Matrices
			////// *** ���� ī�޶� ����ϱ� ���� ī�޶� ��ȯ��� ����  ***
			//  light_position�� framgment shader���� ó���Ǵ�. 
			//  fragment shader������ ī�޶� ��ȯ�� �����ϴ� �κ��� ����.
			//  ���� �̸� ī�޶� ��ȯ ����� ���ؼ� �Ѱ��ش�.
			//  �̶� light_position�� ��ġ�� ��Ÿ���� ����Ʈ�̹Ƿ� �̵�(Translation)��ȯ�� ����ǵ��� �Ѵ�.
			pos = glm::vec3(  view_m * glm::vec4(pos, 1.f) );

			int pos_loc = glGetUniformLocation(g_shader_id, "lights[1].position");
			glUniform3f(pos_loc, pos[0], pos[1], pos[2]);

			
			// ���� ���� ����.
			int intensity_loc = glGetUniformLocation(g_shader_id, "lights[1].intensity");
			glUniform3f(intensity_loc, 1.f, 1.f, 1.f);
		}



		// Spot Light ����.
		{
			// ���� ���� ���� (0: Directionl Light, 1: Point Light, 2: Spot Light), fshader_MultiLights.glsl ����.
			int type_loc = glGetUniformLocation(g_shader_id, "lights[2].type");
			glUniform1i(type_loc, 2);

			// ���� ����ϴ� ��ġ(����) ����.
			glm::vec3 pos(0.f, 300.f, 0.f);
			pos = glm::vec3(  view_m * glm::vec4(pos, 1.f) );

			int pos_loc = glGetUniformLocation(g_shader_id, "lights[2].position");
			glUniform3f(pos_loc, pos[0], pos[1], pos[2]);


			// ���� ���� ����.
			// �ð��� �帧�� ���� ������ ���ϵ��� ��.
			glm::vec3 dir(-1.0f * cos(g_elapsed_time), -1.f, 0.f);
			dir = glm::vec3(  view_m * glm::vec4(dir, 0.f) );

			int dir_loc = glGetUniformLocation(g_shader_id, "lights[2].dir");
			glUniform3f(dir_loc, dir[0], dir[1], dir[2]);
		


			// ���� ���� ���� (������).
			int intensity_loc = glGetUniformLocation(g_shader_id, "lights[2].intensity");
			glUniform3f(intensity_loc, 1.f, 0.f, 0.f);

			// ���� ���� ���� ����.
			int light_cos_cutoff_loc = glGetUniformLocation(g_shader_id, "lights[2].cos_cutoff");
			glUniform1f(light_cos_cutoff_loc, cos( 15.f/180.f * glm::pi<float>() ));
		}

	}




	// Ground
	{
		// Ground�� ���� Phong Shading ���� ���� ���� �����Ѵ�.
		int shininess_loc = glGetUniformLocation(g_shader_id, "shininess_n");
		glUniform1f(shininess_loc, 50.f);

		int K_s_loc = glGetUniformLocation(g_shader_id, "K_s");
		glUniform3f(K_s_loc, 0.3f, 0.3f, 0.3f);

		// ī�޶� ��ȯ ����� �����Ѵ�.
		glm::mat4 modelview_T = view_m;
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(modelview_T));

		// �׸���.
		DrawGround2();
	}

	// Sphere
	{
		// Sphere�� ���� Phong Shading ���� ���� ���� �����Ѵ�.
		int shininess_loc = glGetUniformLocation(g_shader_id, "shininess_n");
		glUniform1f(shininess_loc, 100.f);

		int K_s_loc = glGetUniformLocation(g_shader_id, "K_s");
		glUniform3f(K_s_loc, 0.7f, 0.7f, 0.7f);

		// ��ȯ ����� �����Ѵ�.
		glm::mat4 modelview_T;
		modelview_T = view_m * glm::translate(glm::vec3(0.f, 100.f, 0.f)) * glm::scale(glm::vec3(80.f, 80.f, 80.f));
		glUniformMatrix4fv(m_mat_loc, 1, GL_FALSE,  glm::value_ptr(modelview_T));

		// ��ü �������� ����� Color ���� �����Ѵ�. 
		glVertexAttrib4f(1, 0.3f, 0.6f, 0.9f, 1.f);

		// ���� �׸���.
		DrawSphere();
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
SpeicalKeyboard: ���ڰ����� ǥ���ϱ� ����� Ű���� �Է��� ���� ������ �ڵ����� ȣ��Ǵ� �Լ�. 
�Ķ���� key �� ������ Ű���带 ��Ÿ���� ��ũ�� �� (freeglut_std.h ����).
�Ķ���� x,y�� ���� ���콺 �������� ��ǥ��.
*/
void SpeicalKeyboard(int key, int x, int y )
{
	switch (key)						
	{
	case GLUT_KEY_DOWN:
		std::cout << "down-key pushed" << std::endl;
		break;

	case GLUT_KEY_UP:
		std::cout << "up-key pushed" << std::endl;
		break;

	case GLUT_KEY_LEFT:
		std::cout << "left-key pushed" << std::endl;
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "right-key pushed" << std::endl;
		break;

	}

	glutPostRedisplay();
	return;
}














