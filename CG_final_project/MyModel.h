#pragma once

#include "Angel.h"
#include<vector>
#include <string>
#include <cassert>
#include "mesh.h"
#include "Mesh_Painter.h"
#include "include\TriMesh.h"
using std::vector;
using std::string;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;
typedef vector<point4> pointVec;
typedef vector<color4> colorVec;
typedef vector<float> textureVec;

extern GLuint ModelView;
extern GLuint draw_color;
extern mat4 modelView;
extern GLfloat theta[11];


// 人体的各个部位的高度和宽度等
#define TORSO_HEIGHT 1.0
#define TORSO_WIDTH 0.8
#define UPPER_ARM_HEIGHT 0.4
#define LOWER_ARM_HEIGHT 0.3
#define UPPER_LEG_WIDTH  0.3
#define LOWER_LEG_WIDTH  0.2
#define LOWER_LEG_HEIGHT 0.4
#define UPPER_LEG_HEIGHT 0.3
#define UPPER_ARM_WIDTH  0.3
#define LOWER_ARM_WIDTH  0.2
#define HEAD_HEIGHT 0.5
#define HEAD_WIDTH 0.5
#define NECK_HEIGHT 0.2
#define NECK_WIDTH 0.3
#define HAIR_HEIGHT 0.1
#define HAIR_WIDTH 0.5

// 画一个正方体的所需的顶点数
const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)


// 边长为1的正方形
static point4 vertices[8] = {
	point4(-0.5, -0.5,-0.5, 1.0),
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0)
};

// 常用颜色
static color4 black(0.0, 0.0, 0.0, 1.0);
static color4 red(1.0, 0.0, 0.0, 1.0);
static color4 yellow(1.0, 1.0, 0.0, 1.0);
static color4 green(0.0, 1.0, 0.0, 1.0);
static color4 blue(0.0, 0.0, 1.0, 1.0);
static color4 magenta(1.0, 0.0, 1.0, 1.0);
static color4 cyan(0.0, 1.0, 1.0, 1.0);
static color4 white(1.0, 1.0, 1.0, 1.0);
static color4 OliveDrab(107, 142, 35, 1);

class My_Model
{
private:
	pointVec points;
	colorVec colors;
	pointVec normals;
	textureVec uvs;
	int pointSize;
	int colorSize;
	TriMesh m_TriMesh;
	Mesh_Painter m_mp;
	Mesh_Painter m_obj_mp;

	// 正方体的一个面
	void quad(int a, int b, int c, int d, color4 color);
public:
	My_Model();
	void init_sphere();
	// 绘制边长为1的正方体
	void init_cube(color4 color);
	pointVec get_points();
	colorVec get_colors();
	pointVec get_normals();
	void draw_human();
	void init_grass(string texture_file_name);
	int get_points_size();
	int get_colors_size();
	void draw_mesh();
	void draw_obj_mesh();
	void init_wawa(string objFile = "texture/wawa.obj");
};


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
