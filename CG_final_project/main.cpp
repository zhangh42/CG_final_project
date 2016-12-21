#include <cassert>
#include <vector>
#include "Angel.h"
#include "MyModel.h"
using std::vector;
using std::cout;
using std::endl;

const GLfloat PI = 3.1415926;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "FreeImage.lib")

// ��ģ��
My_Model m_model;


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

mat4 modelView(1.0);
mat4 projection;

vec3 lightPos(0.5, 4, 1.5);
// ������ӰͶӰ���󣬻���ͶӰ֮��������Σ��ú�ɫ��ʾ��
mat4 shadowProjMatrix(
	-lightPos[1], 0, 0, 0,
	lightPos[0], 0, lightPos[2], 1,
	0, 0, -lightPos[1], 0,
	0, 0, 0, -lightPos[1]);

// Set up menu item indices, which we can alos use with the joint angles
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

// Joint angles with initial values
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

GLint angle = Head2;

// ��ת�Ƕȴ�С
const float THETA = 2.0;
float rotationAngle[3] = { 0.0,0.0,0.0 };

// �����᳣��
enum {
	X_axis = 0,
	Y_axis = 1,
	Z_axis = 2
};


// �����������
namespace Camera
{
	/*mat4 modelMatrix(1.0);
	mat4 viewMatrix(1.0);
	mat4 projMatrix(1.0);*/
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
	// ���Ʋ�ƺ
	m_model.init_grass("texture/grass.jpg");
	// ��������
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
	//glUseProgram(0);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	// ��Щ����δ֪ ����
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_LIGHTING);
	glDepthFunc(GL_LESS);		// ��Ȳ��Ժ���
	//glEnable(GL_CULL_FACE);	// ���޳�
	glClearColor(0.0, 0.0, 0.250, 1.0);
}


// ��Ⱦ�������������ͼ�ν���
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	modelView = mat4(1.0);

	/*modelViewMat = RotateX(rotationAngle[X_axis]);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelViewMat);
	glDrawArrays(GL_TRIANGLES, 0, 6);*/
	m_model.draw_mesh();
	modelView = Translate(0, 0, -2) * RotateX(-90) * Scale(0.8, 0.8, 0.8);
	m_model.draw_obj_mesh();

	// �л���ɫ��
	glUseProgram(program);
	glBindVertexArray(vao);

	glUniform3fv(LightPos, 1, &lightPos[0]);

	// ��������
	modelView = Translate(0, 4.5, -3.5);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView);
	// ������ɫ
	glUniform4fv(draw_color, 1, yellow);
	glDrawArrays(GL_TRIANGLES, 36, points.size()); 


	// ������ɫ����
	modelView = Translate(-3, 0.5, -2.5);
	draw_sphere(blue);

	modelView = Scale(1.5, 1.5, 1.5) * Translate(1, 0.5, 1.5);
	draw_sphere(magenta);

	modelView = Scale(1.5, 1.5, 1.5) * Translate(1.5, 0.5, 0);
	draw_sphere(cyan);

	// ������
	modelView = Translate(-2, 0.5, -0.0);
	m_model.draw_human();
	glUniform1i(IsShadow, 1);	// ������Ӱ
	modelView = shadowProjMatrix * modelView;
	m_model.draw_human();
	glUniform1i(IsShadow, 0);	// ȡ��������Ӱ

	glutSwapBuffers();
}

// �����ڴ�С�ı�ʱ���ô˺��������ݳ���仯����һЩ������Ϣ��
// ʹ����tͼ�ν��汣�����ۡ��ṹ��
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat left = -10.0, right = 10.0;
	GLfloat bottom = -5.0, top = 15.0;
	GLfloat zNear = -10.0, zFar = 10.0;

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

	projection = Camera::ortho(-6, 6, -3, 9, -10, 10) * Camera::lookAt(eye, at, up);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

	//model_view = mat4(1.0);   // An Identity matrix
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
	}
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
	case GLUT_KEY_LEFT:
		Camera::phi += PI / 18;
		break;
	case GLUT_KEY_RIGHT:
		Camera::phi -= PI / 18;
		break;
	}
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	reshape(width, height);
	glutPostRedisplay();
}

// ���˵�����
void createMenu()
{
	glutCreateMenu(menu);
	glutAddMenuEntry("torso", Torso);
	glutAddMenuEntry("head1", Head1);
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

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(specialKeyboard);
	createMenu();

	glutMainLoop();
	return 0;
}