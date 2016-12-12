#include "Angel.h"
#include <cassert>

#pragma comment(lib, "glew32.lib")

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// RGBA olors
// һЩ��������ɫ
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 1.0, 1.0),  // white
	color4(0.0, 1.0, 1.0, 1.0)   // cyan
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


	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	// ��Щ����δ֪ ����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_LIGHTING);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glClearColor(1.0, 1.0, 1.0, 1.0);
}

// ��Ⱦ�������������ͼ�ν���
void display()
{

}

// �����ڴ�С�ı�ʱ���ô˺��������ݳ����仯����һЩ������Ϣ��
// ʹ����tͼ�ν��汣�����ۡ��ṹ��
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

	//mat4 projection = Ortho(left, right, bottom, top, zNear, zFar);
	//glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("robot");

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