#include <cassert>
#include <vector>
#include "Angel.h"
#include "MyModel.h"
using std::vector;
using std::cout;
using std::endl;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "FreeImage.lib")

// ��ģ��
My_Model m_model;


GLuint ModelView;
GLuint Projection;
GLuint isTexture;
GLuint texture;

vector<point4> points;
vector<color4> colors;

mat4 modelViewMat(1.0);
mat4 projectionMat(1.0);

// ��ת�Ƕȴ�С
const float THETA = 2.0;
float rotationAngle[3] = { 0.0,0.0,0.0 };
// �����᳣��
enum {
	X_axis = 0,
	Y_axis = 1,
	Z_axis = 2
};

// ����ջ
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

// �����������
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



// ��ʼ����������ʼ��һЩ������Ϣ
void init()
{
	// ��������
	// todo here
	m_model.draw_floor();
	m_model.draw_cube(red);
	points = m_model.get_points();
	colors = m_model.get_colors();



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
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
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
	texture = glGetUniformLocation(program, "texture");
	isTexture = glGetUniformLocation(program, "isTexture");


	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	// ��Щ����δ֪ ����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_LIGHTING);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}


// ��Ⱦ�������������ͼ�ν���
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	modelViewMat = RotateX(rotationAngle[X_axis]);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelViewMat);
	glUniform1i(isTexture, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	modelViewMat = RotateX(rotationAngle[X_axis]) * Translate(0, 0.5, -1.5);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelViewMat);
	glDrawArrays(GL_TRIANGLES, 6, points.size()); 

	//glUniformMatrix4fv(Projection, 1, GL_TRUE, projectionMat);

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
	GLfloat theta = 3.1415 / 4;
	GLfloat phi = 3.1415 / 2;
	/*point4  eye(radius*sin(theta)*cos(phi),
		radius*sin(theta)*sin(phi),
		radius*cos(theta),
		1.0);*/
	point4 eye(0, 1.0, 1, 1.0);
	point4  at(0.0, 0.0, 0.0, 1.0);
	vec4    up(0.0, 1.0, -1.0, 0.0);


	mat4 projection = Ortho(-3, 3, 0, 6, -3, 6) * Camera::lookAt(eye, at, up);
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
	
}

// ��������������Ӧ����
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		
	}else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {

	}else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		
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
	glutCreateWindow("2014150266_�ź�_��ĩ����ҵ");

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