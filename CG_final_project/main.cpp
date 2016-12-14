#include "Angel.h"
#include <cassert>
#include <vector>
using std::vector;
using std::cout;
using std::endl;

#pragma comment(lib, "glew32.lib")

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// 边长为1的正方形
point4 vertices[8] = {
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};

// RGBA olors
// 一些常见的颜色
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(0.0, 1.0, 1.0, 1.0),   // cyan
	color4(1.0, 1.0, 1.0, 1.0)  // white
};

GLuint ModelView;
GLuint Projection;

vector<point4> points;
vector<color4> colors;

// 矩阵栈
class MatrixStack {
	int    _index;
	int    _size;
	mat4*  _matrices;

public:
	MatrixStack(int numMatrices = 100) :_index(0), _size(numMatrices)
	{
		_matrices = new mat4[numMatrices];
	}

	~MatrixStack()
	{
		delete[]_matrices;
	}

	void push(const mat4& m) {
		assert(_index + 1 < _size);
		_matrices[_index++] = m;

	}

	mat4& pop(void) {
		assert(_index - 1 >= 0);
		_index--;
		return _matrices[_index];
	}
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
	



	// Create a vertex array object
	GLuint vao;
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
	GLuint program = InitShader("vshader82.glsl", "fshader82.glsl");
	glUseProgram(program);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * points.size()));

	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");


	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	// 这些作用未知 ？？
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

// 渲染函数，负责更新图形界面
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


//	glUniformMatrix4fv(ModelView, 1, GL_TRUE, instance);//父节点矩阵*本节点局部变换矩阵
//	glDrawArrays(GL_TRIANGLES, 0, points.size()); 

//	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

	glutSwapBuffers();
}

// 当窗口大小改变时调用此函数，根据长宽变化调整一些参数信息，
// 使整个t图形界面保持美观、结构等
void reshape(int width, int height)
{
	//glViewport(0, 0, width, height);

	//GLfloat left = -10.0, right = 10.0;
	//GLfloat bottom = -5.0, top = 15.0;
	//GLfloat zNear = -10.0, zFar = 10.0;

	//GLfloat aspect = GLfloat(width) / height;

	//if (aspect > 1.0) {
	//	left *= aspect;
	//	right *= aspect;
	//}
	//else {
	//	bottom /= aspect;
	//	top /= aspect;
	//}

	// mat4 projection = Ortho(left, right, bottom, top, zNear, zFar);
	//glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

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



	glutMainLoop();
	return 0;
}