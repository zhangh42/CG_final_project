#include "MyModel.h"



// 正方体的一个面
void My_Model::quad(int a, int b, int c, int d, color4 color)
{
	points.push_back(vertices[a]);
	points.push_back(vertices[b]);
	points.push_back(vertices[c]);
	points.push_back(vertices[b]);
	points.push_back(vertices[c]);
	points.push_back(vertices[d]);
	for (int i = 0;i < 6;i++)
		colors.push_back(color);
}

// 绘制边长为1的正方体
void My_Model::draw_cube(color4 color)
{
	quad(0, 1, 2, 3, color);
	quad(4, 5, 6, 7, color);
	quad(0, 1, 4, 5, color);
	quad(2, 3, 6, 7, color);
	quad(0, 2, 4, 6, color);
	quad(1, 3, 5, 7, color);
}

// 绘制地板
void My_Model::draw_floor()
{
	point4 p1(-10, 0, -10, 1.0);
	point4 p2(-10, 0, 10, 1.0);
	point4 p3(10, 0, -10, 1.0);
	point4 p4(10, 0, 10, 1.0);

	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	points.push_back(p2);
	points.push_back(p3);
	points.push_back(p4);
	for (int i = 0;i < 6;i++)
		colors.push_back(green);
}

pointVec My_Model::get_points()
{
	return points;
}

colorVec My_Model::get_colors()
{
	return colors;
}
