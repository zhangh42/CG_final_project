#pragma once

#include "Angel.h"
#include<vector>
using std::vector;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;
typedef vector<point4> pointVec;
typedef vector<color4> colorVec;

// �߳�Ϊ1��������
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

// ������ɫ
static color4 black(0.0, 0.0, 0.0, 1.0);
static color4 red(1.0, 0.0, 0.0, 1.0);
static color4 yellow(1.0, 1.0, 0.0, 1.0);
static color4 green(0.0, 1.0, 0.0, 1.0);
static color4 blue(0.0, 0.0, 1.0, 1.0);
static color4 magenta(1.0, 0.0, 1.0, 1.0);
static color4 cyan(0.0, 1.0, 1.0, 1.0);
static color4 white(1.0, 1.0, 1.0, 1.0);

class My_Model
{
private:
	pointVec points;
	colorVec colors;

	// �������һ����
	void quad(int a, int b, int c, int d, color4 color);
public:
	My_Model() {}
	void draw_cube(color4 color);
	void draw_floor();
	pointVec get_points();
	colorVec get_colors();
};


