#include "MyModel.h"

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

void
torso()
{
	// mvstack.push( model_view );//保存父节点矩阵

	mat4 instance = (Translate(0.0, 0.5 * TORSO_HEIGHT, 0.0) *
		Scale(TORSO_WIDTH, TORSO_HEIGHT, TORSO_WIDTH));//本节点局部变换矩阵

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, blue);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	//glUniform4fv(draw_color, 1, color_torso);
	// model_view = mvstack.pop();//恢复父节点矩阵
}

void
head()
{
	mat4 instance = (Translate(0.0, 0.5 * HEAD_HEIGHT, 0.0) *
		Scale(HEAD_WIDTH, HEAD_HEIGHT, HEAD_WIDTH));//本节点局部变换矩阵

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, cyan);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void
neck()
{
	mat4 instance = (Translate(0.0, 0.5 * NECK_HEIGHT, 0.0) *
		Scale(NECK_WIDTH, NECK_HEIGHT, NECK_WIDTH));//本节点局部变换矩阵

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, cyan);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	//glUniform4fv(draw_color, 1, color_torso);
}

void hair()
{
	mat4 instance = (Translate(0.0, 0.5 * HAIR_HEIGHT, 0.0) *
		Scale(HAIR_WIDTH, HAIR_HEIGHT, HAIR_WIDTH));//本节点局部变换矩阵

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, black);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	//glUniform4fv(draw_color, 1, color_torso);
}

void
left_upper_arm()
{
	mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
		Scale(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, OliveDrab);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void
left_lower_arm()
{
	mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
		Scale(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, black);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void
right_upper_arm()
{
	mat4 instance = (Translate(0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
		Scale(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, OliveDrab);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void
right_lower_arm()
{
	mat4 instance = (Translate(0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0) *
		Scale(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, black);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void
left_upper_leg()
{
	mat4 instance = (Translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *
		Scale(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, black);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void
left_lower_leg()
{
	mat4 instance = (Translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *
		Scale(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, black);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void
right_upper_leg()
{
	mat4 instance = (Translate(0.0, 0.5 * UPPER_LEG_HEIGHT, 0.0) *
		Scale(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, black);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void
right_lower_leg()
{
	mat4 instance = (Translate(0.0, 0.5 * LOWER_LEG_HEIGHT, 0.0) *
		Scale(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH));//本节点局部变换矩阵
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);//父节点矩阵*本节点局部变换矩阵
	glUniform4fv(draw_color, 1, black);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

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
		points.push_back(point4(v[f[i].a]));
		points.push_back(point4(v[f[i].b]));
		points.push_back(point4(v[f[i].c]));

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
	MatrixStack mvstack;
	mvstack.push(modelView);
	modelView = RotateY(theta[Torso]) * Translate(0,0.5,-3.0) ;//躯干变换矩阵
	torso();//躯干绘制

	mvstack.push(modelView);//保存躯干变换矩阵
	modelView *= Translate(0, TORSO_HEIGHT, 0);
	neck();//脖子绘制
	modelView *= Translate(0, NECK_HEIGHT, 0) * RotateY(theta[Head1]);
	head();//头部绘制
	modelView *= Translate(0, HEAD_HEIGHT, 0);
	hair();//头部绘制
	modelView = mvstack.pop();//恢复躯干变换矩阵

	mvstack.push(modelView); //保存躯干变换矩阵
	modelView *= Translate(-0.5*TORSO_WIDTH - 0.5*UPPER_ARM_WIDTH, TORSO_HEIGHT, 0) * RotateX(theta[LeftUpperArm]);
	left_upper_arm();//左上臂绘制
	modelView *= Translate(0, UPPER_ARM_HEIGHT, 0) * RotateX(theta[LeftLowerArm]);
	left_lower_arm();//左下臂绘制
	modelView = mvstack.pop();//恢复躯干变换矩阵

	mvstack.push(modelView); //保存躯干变换矩阵
	modelView *= Translate(0.5*TORSO_WIDTH + 0.5*UPPER_ARM_WIDTH, TORSO_HEIGHT, 0) * RotateX(theta[RightUpperArm]);
	right_upper_arm();//右上臂绘制
	modelView *= Translate(0, UPPER_ARM_HEIGHT, 0) * RotateX(theta[RightLowerArm]);
	right_lower_arm();//右下臂绘制
	modelView = mvstack.pop();//恢复躯干变换矩阵

	mvstack.push(modelView); //保存躯干变换矩阵
	modelView *= Translate(-0.25*TORSO_WIDTH, 0, 0) * RotateX(theta[LeftUpperLeg]);
	left_upper_leg();//左上腿绘制
	modelView *= Translate(0, UPPER_LEG_HEIGHT, 0) *
		RotateX(theta[LeftLowerLeg]);
	left_lower_leg();//左下腿绘制
	modelView = mvstack.pop();//恢复躯干变换矩阵

	mvstack.push(modelView); //保存躯干变换矩阵
	modelView *= Translate(0.25*TORSO_WIDTH, 0, 0) * RotateX(theta[RightUpperLeg]);
	right_upper_leg();//右上腿绘制
	modelView *= Translate(0, UPPER_LEG_HEIGHT, 0) *
		RotateX(theta[RightLowerLeg]);
	right_lower_leg();//右下腿绘制

	modelView = mvstack.pop();//恢复父节点矩阵
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
