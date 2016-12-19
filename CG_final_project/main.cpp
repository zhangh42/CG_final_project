#include <cassert>
#include <vector>
#include "Angel.h"
#include "MyModel.h"
using std::vector;
using std::cout;
using std::endl;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "FreeImage.lib")

// 建模类
My_Model m_model;


GLuint ModelView;
GLuint Projection;
GLuint LightPos;
GLuint program;
GLuint draw_color;
GLuint vao;

vector<point4> points;
vector<color4> colors;
vector<point4> normals;

mat4 modelView(1.0);
mat4 projection;

vec3 lightPos(0, 4, -3.5);

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

// 旋转角度大小
const float THETA = 2.0;
float rotationAngle[3] = { 0.0,0.0,0.0 };
// 坐标轴常量
enum {
	X_axis = 0,
	Y_axis = 1,
	Z_axis = 2
};


// 相机参数设置
namespace Camera
{
	/*mat4 modelMatrix(1.0);
	mat4 viewMatrix(1.0);
	mat4 projMatrix(1.0);*/

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



// 初始化函数，初始化一些参数信息
void init()
{
	// 顶点生成
	// todo here
	m_model.draw_grass("texture/grass.jpg");
	//m_model.draw_floor();
	m_model.draw_cube(red);
	m_model.draw_lamb();
	points = m_model.get_points();
	colors = m_model.get_colors();



	// Create a vertex array object
	vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * points.size() + sizeof(color4) * colors.size(),
		NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * points.size(), sizeof(color4) * colors.size(),
		&colors[0]);

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

	// 法向量
	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * normals.size()));


	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");
	draw_color = glGetUniformLocation(program, "draw_color");
	LightPos = glGetUniformLocation(program, "lightPos");
	//glUseProgram(0);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	// 这些作用未知 ？？
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_LIGHTING);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.250, 1.0);
}


// 渲染函数，负责更新图形界面
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelView = RotateY(rotationAngle[X_axis]);

	/*modelViewMat = RotateX(rotationAngle[X_axis]);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelViewMat);
	glDrawArrays(GL_TRIANGLES, 0, 6);*/
	m_model.draw_mesh();

	glUseProgram(program);
	glBindVertexArray(vao);

	glUniform3fv(LightPos, 1, &lightPos[0]);

	// 绘制月亮
	modelView = Translate(0, 4.5, -3.5);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView);
	// 绘制颜色
	glUniform4fv(draw_color, 1, yellow);
	glDrawArrays(GL_TRIANGLES, 36, points.size()); 

	// 绘制人
	modelView = RotateY(rotationAngle[X_axis])*Translate(0, 0.5, -3.0);
	m_model.draw_human();


	// 计算阴影投影矩阵，绘制投影之后的三角形（用黑色表示）
	mat4 shadowProjMatrix(
		-lightPos[1], 0, 0, 0,
		lightPos[0], 0, lightPos[2], 1,
		0, 0, -lightPos[1], 0,
		0, 0, 0, -lightPos[1]);
	// 绘制人

	modelView =shadowProjMatrix*Translate(0, 0.5, -3.0);
	//modelView = RotateY(rotationAngle[X_axis])*Translate(-2, 0.5, -4.50);
	m_model.draw_human();

	glutSwapBuffers();
}

// 当窗口大小改变时调用此函数，根据长宽变化调整一些参数信息，
// 使整个t图形界面保持美观、结构等
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
	GLfloat theta = 3.1415 / 4;
	GLfloat phi = 3.1415 / 2;
	/*point4  eye(radius*sin(theta)*cos(phi),
		radius*sin(theta)*sin(phi),
		radius*cos(theta),
		1.0);*/
	point4 eye(0, 0.50, 1.0, 1.0);
	point4  at(0.0, 0.0, 0.0, 1.0);
	vec4    up(0.0, 1.0, -1.0, 0.0);

	projection = Ortho(-3, 3, 0, 6, -3, 6) * Camera::lookAt(eye, at, up);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

	//model_view = mat4(1.0);   // An Identity matrix
}

// 键盘函数
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

// 菜单函数
void menu(int option)
{
	
}

// 鼠标的三个键的相应函数
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		
	}else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {

	}else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		
	}

	// 调用display函数，刷新界面
	glutPostRedisplay();
}


// 键盘上下左右键响应函数
void specialKeyboard(int key, int x, int y)
{

	switch (key)
	{
	case GLUT_KEY_DOWN:
		rotationAngle[X_axis] += THETA;
		break;
	case GLUT_KEY_UP:
		rotationAngle[X_axis] -= THETA;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("2014150266_张航_期末大作业");

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(specialKeyboard);

	glutMainLoop();
	return 0;
}