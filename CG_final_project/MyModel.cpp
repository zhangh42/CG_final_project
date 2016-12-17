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

My_Model::My_Model()
{
}

void My_Model::draw_lamb()
{
	m_TriMesh.read_off("sphere.off");
	auto v = m_TriMesh.v();
	auto f = m_TriMesh.f();

	for (int i = 0;i < f.size();i++)
	{
		points.push_back(point4(v[f[i].a]) + point4(0, 3, -3, 0));
		points.push_back(point4(v[f[i].b]) + point4(0, 3, -3, 0));
		points.push_back(point4(v[f[i].c]) + point4(0, 3, -3, 0));

		colors.push_back(yellow);
		colors.push_back(yellow);
		colors.push_back(yellow);
	}
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

	pointSize += 36;
	colorSize += 36;
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

	pointSize += 6;
	colorSize += 6;
}

pointVec My_Model::get_points()
{
	return points;
}

colorVec My_Model::get_colors()
{
	return colors;
}

void My_Model::draw_human()
{
}

void My_Model::draw_grass(string texture_file_name)
{
	My_Mesh* my_mesh1 = new My_Mesh;
	my_mesh1->generate_floor();//生成地板
	my_mesh1->set_texture_file(texture_file_name);//指定纹理图像文件
	my_mesh1->set_translate(0, 0, 0);
//	my_mesh1->set_theta(45, 45, 45.);//旋转轴
//	my_mesh1->set_theta_step(1, 1, 1);//旋转速度
	m_mp.add_mesh(my_mesh1);


	m_mp.init_shaders("v_texture.glsl", "f_texture.glsl");
	m_mp.update_vertex_buffer();
	m_mp.update_texture();
}

int My_Model::get_points_size()
{
	return pointSize;
}

int My_Model::get_colors_size()
{
	return colorSize;
}

void My_Model::draw_mesh()
{
	m_mp.draw_meshes();
}
