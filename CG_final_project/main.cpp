#include <cassert>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Angel.h"
#include "MyModel.h"
#include "main.h"
using std::vector;
using std::cout;
using std::endl;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "FreeImage.lib")

const GLfloat PI = 3.1415926;

// ��ģ��
My_Model m_model;

// ������ɫ��������Ӧ��id
GLuint ShadowMatrix;
GLuint ModelView;
GLuint Projection;
GLuint LightPos;
GLuint IsShadow;
GLuint program;
GLuint draw_color;
GLuint vao;

vector<point4> points;
vector<color4> colors;
vector<point4> normals;

mat4 modelView;
mat4 projection;

vec3 lightPos(0.5, 10, 0.5);
// ������ӰͶӰ���󣬻���ͶӰ֮��������Σ��ú�ɫ��ʾ��
mat4 shadowProjMatrix(
	-lightPos[1], 0, 0, 0,
	lightPos[0], 0, lightPos[2], 1,
	0, 0, -lightPos[1], 0,
	0, 0, 0, -lightPos[1]);

// ���������������λ��Ӧ��������
enum {
	Torso,
	Head1,
	Head2,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm,
	RightUpperLeg,
	RightLowerLeg,
	LeftUpperLeg,
	LeftLowerLeg,
	NumJointAngles,
	Quit
};

// �����˸�����λ�ĳ�ʼ�Ƕ�
GLfloat theta[NumJointAngles] = {
	0.0,    // Torso
	0.0,    // Head1
	0.0,    // Head2
	0.0,    // RightUpperArm
	0.0,    // RightLowerArm
	0.0,    // LeftUpperArm
	0.0,    // LeftLowerArm
	180.0,  // RightUpperLeg
	0.0,    // RightLowerLeg
	180.0,  // LeftUpperLeg
	0.0     // LeftLowerLeg
};
// ����ָ�������˵�ǰת���Ĳ�λ
GLint angle = Head2;

// ���޵��ƶ�����
vec3f wawa_translation(0.0);

// �����ƶ�����
vec3f balloon1_translation(0.0);
vec3f balloon2_translation(0.0);

// ������y�ƶ�����
vec3f human_translateion(0.0);

// �����᳣��
enum {
	X_axis = 0,
	Y_axis = 1,
	Z_axis = 2
};


// �����������
namespace Camera
{
	// ŷ���������������л�
	GLfloat theta = PI / 4;	// ��x�����ת�Ƕ�
	GLfloat phi = PI / 2;	// ��y��ĽǶ�

	mat4 ortho(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar)
	{
		mat4 m;
		m[0][0] = 2 / (right - left);
		m[0][3] = -(right + left) / (right - left);
		m[1][1] = 2 / (top - bottom);
		m[1][3] = -(top + bottom) / (top - bottom);
		m[2][2] = -2 / (zFar - zNear);
		m[2][3] = -(zFar + zNear) / (zFar - zNear);
		m[3][3] = 1;
		return m;
	}

	mat4 perspective(const GLfloat fovy, const GLfloat aspect,
		const GLfloat zNear, const GLfloat zFar)
	{
		GLfloat top = tan(fovy * DegreesToRadians / 2) * zNear;
		GLfloat right = top * aspect;

		mat4 m;
		m[0][0] = zNear / right;
		m[1][1] = zNear / top;
		m[2][2] = -(zFar + zNear) / (zFar - zNear);
		m[2][3] = -(2 * zFar * zNear) / (zFar - zNear);
		m[3][2] = -1;
		return m;
	}

	mat4 lookAt(const vec4& eye, const vec4& at, const vec4& up)
	{
		vec4 n = normalize(eye - at);
		vec3 uu = normalize(cross(up, n));
		vec4 u = vec4(uu.x, uu.y, uu.z, 0.0);
		vec3 vv = normalize(cross(n, u));
		vec4 v = vec4(vv.x, vv.y, vv.z, 0.0);
		vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
		mat4 c = mat4(u, v, n, t);
		return c * Translate(-eye);
	}
}

// ����һ��������Ӱ����
void draw_sphere(color4 color)
{
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView);
	glUniform4fv(draw_color, 1, color);	// ������ɫ
	glDrawArrays(GL_TRIANGLES, 36, points.size());
	// ������Ӱ
	modelView = shadowProjMatrix * modelView;
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView);
	glUniform1i(IsShadow, 1);	// ������Ӱ
	glUniform4fv(draw_color, 1, black);
	glDrawArrays(GL_TRIANGLES, 36, points.size());
	glUniform1i(IsShadow, 0);	// ȡ��������Ӱ
}

// ��ʼ����������ʼ��һЩ������Ϣ
void init()
{
	// ��������
	// ��ƺ���ݳ�ʼ��
	m_model.init_grass("texture/grass.jpg");
	// �������ݳ�ʼ��
	m_model.init_wawa("texture/wawa.obj");

	m_model.init_cube(red);
	m_model.init_sphere();
	points = m_model.get_points();
	colors = m_model.get_colors();
	normals = m_model.get_normals();

	// Create a vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * points.size() + sizeof(color4) * colors.size() +
		sizeof(point4) * normals.size(), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * points.size(), sizeof(color4) * colors.size(),
		&colors[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * points.size() + sizeof(color4) * colors.size(),
		sizeof(point4) * normals.size(), &normals[0]);

	// Load shaders and use the resulting shader program
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * points.size()));

	// ������
	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * points.size() + sizeof(color4) * colors.size()));


	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");
	draw_color = glGetUniformLocation(program, "draw_color");
	LightPos = glGetUniformLocation(program, "lightPos");
	IsShadow = glGetUniformLocation(program, "isShadow");
	ShadowMatrix = glGetUniformLocation(program, "shadowMatrix");
	//glUseProgram(0);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glDepthFunc(GL_LESS);		// ��Ȳ��Ժ���
	//glEnable(GL_CULL_FACE);	// ���޳�
	glClearColor(0.0, 0.0, 0.250, 1.0);
}


// ��Ⱦ�������������ͼ�ν���
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	modelView = mat4(1.0);
	// ����ƺ
	m_model.draw_mesh();
	modelView = Translate(wawa_translation) // �û������ƶ�
		* Translate(0, 0.5, -2) * RotateX(-90) * Scale(0.8, 0.8, 0.8);	// ��ʼ����
	// ��������
	m_model.draw_obj_mesh();

	// �л���ɫ��
	glUseProgram(program);
	glBindVertexArray(vao);

	glUniform3fv(LightPos, 1, &lightPos[0]);

	// ��������
	modelView = Translate(0, 5.5, -3.5);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView);
	// ������ɫ
	glUniform4fv(draw_color, 1, yellow);
	glDrawArrays(GL_TRIANGLES, 36, points.size()); 

	// ������ɫ������ƶ�����
	modelView = Translate(balloon1_translation) * Translate(0, 0.5, 2);
	draw_sphere(red);

	// ������ɫ������ƶ�����
	modelView = Translate(balloon2_translation) * Translate(0, 0.5, -2);
	draw_sphere(green);


	// ������ɫ����
	modelView = Translate(-3, 0.5, -2.5);
	draw_sphere(blue);
	// ����magentaɫ����
	modelView = Scale(1.5, 1.5, 1.5) * Translate(1, 0.5, 1.5);
	draw_sphere(magenta);
	// ����cyanɫ����
	modelView = Scale(1.5, 1.5, 1.5) * Translate(1.5, 0.5, 0);
	draw_sphere(cyan);

	// �����ˣ���ʼ����
	modelView = Scale(1.5, 1.5, 1.5) * Translate(-2, 0.5, -0.0);
	// ���̿����ƶ�������
	modelView = Translate(human_translateion) * modelView;
	m_model.draw_human();
	glUniform1i(IsShadow, 1);	// ������Ӱ
	modelView = shadowProjMatrix * modelView;
	m_model.draw_human();
	glUniform1i(IsShadow, 0);	// ֹͣ������Ӱ

	 glutSwapBuffers();
}

// �����ڴ�С�ı�ʱ���ô˺��������ݳ���仯����һЩ������Ϣ��
// ʹ����tͼ�ν��汣�����ۡ��ṹ��
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat left = -6.0, right = 6.0;
	GLfloat bottom = -3.0, top = 9.0;
	GLfloat zNear = -6.0, zFar = 6.0;

	// �����Ӵ��壬ʹ֮��Ӧ���ڶ�������
	GLfloat aspect = GLfloat(width) / height;
	if (aspect > 1.0) {
		left *= aspect;
		right *= aspect;
	}
	else {
		bottom /= aspect;
		top /= aspect;
	}
	
	GLfloat radius = 1.0;
	using Camera::phi;
	using Camera::theta;
	// �������ŷ����
	point4  eye(radius*cos(theta)*cos(phi),
		radius*sin(theta),
		radius*cos(theta)*sin(phi),
		1.0);
	point4  at(0.0, 0.0, 0.0, 1.0);
	vec4    up(0.0, 1.0, 0.0, 0.0);

	projection = Camera::ortho(left, right, bottom, top, zNear, zFar) *  Camera::lookAt(eye, at, up);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

}

// ���̺���
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;

	// �����ƶ�����
	case 'w':
		wawa_translation.z -= 0.1;
		break;
	case 's':
		wawa_translation.z += 0.1;
		break;
	case 'a':
		wawa_translation.x -= 0.1;
		break;
	case 'd':
		wawa_translation.x += 0.1;
		break;

	// �������ƶ�����
	case 'i':
		human_translateion.z -= 0.1;
		break;
	case 'k':
		human_translateion.z += 0.1;
		break;
	case 'j':
		human_translateion.x -= 0.1;
		break;
	case 'l':
		human_translateion.x += 0.1;
		break;
	}

	glutPostRedisplay();
}

// �˵�����
void menu(int option)
{
	if (option == Quit) {
		exit(EXIT_SUCCESS);
	}

	angle = option;
}

// ��������������Ӧ����
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		theta[angle] += 5.0;
		if (theta[angle] > 360.0) { theta[angle] -= 360.0; }
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	{
		theta[angle] -= 5.0;
		if (theta[angle] < 0.0) { theta[angle] += 360.0; }
	}

	// ����display������ˢ�½���
	glutPostRedisplay();
}


// �����������Ҽ���Ӧ����
void specialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	// ��������������ƶ�
	case GLUT_KEY_DOWN:
		Camera::theta += PI / 18;
		if (Camera::theta >= PI / 2)
			Camera::theta = PI / 2;
		break;
	case GLUT_KEY_UP:
		Camera::theta -= PI / 18;
		if (Camera::theta < 0)
			Camera::theta = 0;
		break;

	// ��������������ƶ�
	case GLUT_KEY_RIGHT:
		Camera::phi += PI / 18;
		break;
	case GLUT_KEY_LEFT:
		Camera::phi -= PI / 18;
		break;
	}

	// ����projection������任������reshape����Ե���reshape�ػ�
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	reshape(width, height);
	glutPostRedisplay();
}

// ���˵�����
void createMenu()
{
	// �������ƻ����˵Ĳ˵�
	glutCreateMenu(menu);
	glutAddMenuEntry("torso", Torso);
	glutAddMenuEntry("head", Head1);
//	glutAddMenuEntry("head2", Head2);
	glutAddMenuEntry("right_upper_arm", RightUpperArm);
	glutAddMenuEntry("right_lower_arm", RightLowerArm);
	glutAddMenuEntry("left_upper_arm", LeftUpperArm);
	glutAddMenuEntry("left_lower_arm", LeftLowerArm);
	glutAddMenuEntry("right_upper_leg", RightUpperLeg);
	glutAddMenuEntry("right_lower_leg", RightLowerLeg);
	glutAddMenuEntry("left_upper_leg", LeftUpperLeg);
	glutAddMenuEntry("left_lower_leg", LeftLowerLeg);
	glutAddMenuEntry("quit", Quit);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

// ����
void idle()
{
	static int lasttime = clock();
	if (clock() - lasttime > 500) // ÿ��500ms����һ��
	{
		lasttime = clock();
		// ����0��5������������Ҹ����������С�ƶ�����
		// �Ӷ��õ���������ƶ��Ķ���
		int f = rand() % 6;
		if (f == 0) {
			balloon1_translation.x -= 0.5;
			balloon2_translation.x += 0.3;
		}
		else if (f == 1) {
			balloon1_translation.x += 0.3;
			balloon2_translation.x -= 0.5;
		}
		else if (f == 2) {
			balloon1_translation.z -= 0.5;
			balloon2_translation.z += 0.5;
		}
		else if (f == 3) {
			balloon1_translation.z += 0.5;
			balloon2_translation.x += 0.3;
		}
		else if (f == 4) {
			balloon1_translation.x += 0.3;
			balloon2_translation.x += 0.3;
		}
		else if (f == 5) {
			balloon1_translation.x += 0.3;
			balloon2_translation.z -= 0.5;
		}

		// ����Ƿ񳬹��޶���Χ
		if (balloon1_translation.x < -5)
			balloon1_translation.x = 5;
		else if (balloon1_translation.x > 5)
			balloon1_translation.x = -5;
		if (balloon1_translation.z < -5)
			balloon1_translation.z = 5;
		else if (balloon1_translation.z > 5)
			balloon1_translation.z = -5;

		if (balloon2_translation.x < -5)
			balloon2_translation.x = 5;
		else if (balloon2_translation.x > 5)
			balloon2_translation.x = -5;
		if (balloon2_translation.z < -5)
			balloon2_translation.z = 5;
		else if (balloon2_translation.z > 5)
			balloon2_translation.z = -5;

		glutPostRedisplay();
	}

	
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("2014150266_�ź�_��ĩ����ҵ");

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	srand(time(0));
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(specialKeyboard);
	createMenu();	

	glutMainLoop();
	return 0;
}