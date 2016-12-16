#pragma once

#include "Angel.h"
#include<vector>
#include <string>
using std::vector;
using std::string;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;
typedef vector<point4> pointVec;
typedef vector<color4> colorVec;
typedef vector<float> textureVec;

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

class My_Model
{
private:
	pointVec points;
	colorVec colors;
	textureVec uvs;
	int pointSize;
	int colorSize;

	// 正方体的一个面
	void quad(int a, int b, int c, int d, color4 color);
public:
	My_Model();
	void draw_cube(color4 color);
	void draw_floor();
	pointVec get_points();
	colorVec get_colors();
	void draw_human();
	void draw_grass(string texture_file_name);
	int get_points_size();
	int get_colors_size();
};


