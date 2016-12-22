#include "mesh.h"
#include<sstream>
#include <fstream>
#include <iosfwd>
#include <algorithm>
#include <gl/GL.h>
#include <math.h>
#include <algorithm>


My_Mesh::My_Mesh()
{
	vTranslation[0] = Theta[0] = 0;
	vTranslation[1] = Theta[1] = 0;
	vTranslation[2] = Theta[2] = 0;
	Theta[0] = 45;
}


My_Mesh::~My_Mesh()
{
	
}


void My_Mesh::normal_to_color(float nx, float ny, float nz, float& r, float& g, float& b)
{
	r = float(std::min(std::max(0.5 * (nx + 1.0), 0.0), 1.0));
	g = float(std::min(std::max(0.5 * (ny + 1.0), 0.0), 1.0));
	b = float(std::min(std::max(0.5 * (nz + 1.0), 0.0), 1.0));
};

const VtList&  My_Mesh::get_vts()
{
	return this->m_vt_list_;
};
void My_Mesh::clear_data()
{
	m_vertices_.clear();
	m_normals_.clear();
	m_faces_.clear();
	m_color_list_.clear();
	m_vt_list_.clear();
};
void My_Mesh::get_boundingbox(point3f& min_p, point3f& max_p) const
{
	min_p = this->m_min_box_;
	max_p = this->m_max_box_;
};
const STLVectorf&  My_Mesh::get_colors()
{
	return this->m_color_list_;
};
const VertexList& My_Mesh::get_vertices()
{
	return this->m_vertices_;
};
const NormalList& My_Mesh::get_normals()
{
	return this->m_normals_;
};
const FaceList&   My_Mesh::get_faces()
{
	return this->m_faces_;
};

int My_Mesh::num_faces()
{
	// obj�ļ���Ҫ���⴦��
	if (isObjMesh)
		return this->m_faces_.size() / 3 / 3;
	return this->m_faces_.size()/3;
};


int My_Mesh::num_vertices()
{
	return this->m_vertices_.size()/3;
};

const point3f& My_Mesh::get_center()
{
	return this->m_center_;
};

// ���ɵذ壬����ƺ
void My_Mesh::generate_floor()
{
	const int width = 10;
	this->clear_data();
	this->m_center_ = point3f(0, 0, 0);

	// �ĸ����㣬�����������ι���һ��������
	// Ϊ�˱�������Ӱ������ȳ�ͻ����y��������Ϊ-0.01
	m_vertices_.push_back(-6.0);
	m_vertices_.push_back(-0.01);
	m_vertices_.push_back(6.0);

	m_vertices_.push_back(6.0);
	m_vertices_.push_back(-0.01);
	m_vertices_.push_back(6.0);

	m_vertices_.push_back(-6.0);
	m_vertices_.push_back(-0.01);
	m_vertices_.push_back(-6.0);	

	m_vertices_.push_back(6.0);
	m_vertices_.push_back(-0.01);
	m_vertices_.push_back(-6.0);
		
	
	// ���뷨�������Ҹ��ݷ�����������ɫ
	for (int i = 0; i < 4; i++)
	{
		m_normals_.push_back(0.0);
		m_normals_.push_back(1.0);
		m_normals_.push_back(0.0);

		//������÷�����������ɫ��ѧ�������Զ����Լ�����ɫ���ɷ�ʽ
		float r;
		float g;
		float b;
		My_Mesh::normal_to_color(0.0, 1.0, 0.0, r, g, b);
		m_color_list_.push_back(r);
		m_color_list_.push_back(g);
		m_color_list_.push_back(b);
	}

	// �ذ���һ�������Σ�ֻ������������Ƭ��
		m_faces_.push_back(0);
		m_faces_.push_back(1);
		m_faces_.push_back(2);
		//��������
		m_vt_list_.push_back(0.0);
		m_vt_list_.push_back(0.0);
		m_vt_list_.push_back(1.0);
		m_vt_list_.push_back(0.0);
		m_vt_list_.push_back(0.0);
		m_vt_list_.push_back(1.0);

		m_faces_.push_back(1);
		m_faces_.push_back(2);
		m_faces_.push_back(3);
		m_vt_list_.push_back(1.0);
		m_vt_list_.push_back(0.0);
		m_vt_list_.push_back(0.0);
		m_vt_list_.push_back(1.0);
		m_vt_list_.push_back(1.0);
		m_vt_list_.push_back(1.0);
};

// ��������obj
void My_Mesh::load_obj(std::string obj_File)
{
	//���ڴ���Ӵ���ʵ�ֶԺ���UV�����obj�ļ��Ķ�ȡ
	std::ifstream infile(obj_File);
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string type;
		int xi, yi, zi;
		GLfloat x, y, z;
		iss >> type;
		// ע����Ϣ
		if (type == "#")
			continue;
		else if (type == "v")
		{
			// ��������
			iss >> x >> y >> z;
			m_vertices_.push_back(x);
			m_vertices_.push_back(y);
			m_vertices_.push_back(z);
		}
		// Ƭ����Ϣ
		else if (type == "f")
		{
			// ������Ƭ��������������������
			char t;
			for (int i = 0;i < 3;i++)
			{
				iss >> xi >> t >> yi >> t >> zi;
				// ����Ԥ�ȼ�һ����Ϊ�����Ǵ�0��ʼ��
				m_faces_.push_back(xi - 1);
				m_faces_.push_back(yi - 1);
				m_faces_.push_back(zi - 1);
			}
		}
		// ���ߺ���ɫ
		else if (type == "vn")
		{
			iss >> x >> y >> z;
			m_normals_.push_back(x);
			m_normals_.push_back(y);
			m_normals_.push_back(z);

			//������÷�����������ɫ�������Զ����Լ�����ɫ���ɷ�ʽ
			float r;
			float g;
			float b;
			My_Mesh::normal_to_color(x, y, z, r, g, b);
			m_color_list_.push_back(r);
			m_color_list_.push_back(g);
			m_color_list_.push_back(b);
		}
		// ��ȡuv����
		else if (type == "vt")
		{
			iss >> x >> y;
			m_vt_list_.push_back(x);
			m_vt_list_.push_back(y);
		}
	}

};

void My_Mesh::set_texture_file(std::string s)
{
	this->texture_file_name = s;
};
std::string My_Mesh::get_texture_file()
{
	return this->texture_file_name;
};

void My_Mesh::set_translate(float x, float y, float z)
{
	vTranslation[0] = x;
	vTranslation[1] = y;
	vTranslation[2] = z;

};
void My_Mesh::get_translate(float& x, float& y, float& z)
{
	x = vTranslation[0];
	y = vTranslation[1];
	z = vTranslation[2];
};

void My_Mesh::set_theta(float x, float y, float z)
{
	Theta[0] = x;
	Theta[1] = y;
	Theta[2] = z;
};
void My_Mesh::get_theta(float& x, float& y, float& z)
{
	x = Theta[0];
	y = Theta[1];
	z = Theta[2];
};

void My_Mesh::set_theta_step(float x, float y, float z)
{
	Theta_step[0] = x;
	Theta_step[1] = y;
	Theta_step[2] = z;
};

void My_Mesh::add_theta_step()
{
	Theta[0] = Theta[0] + Theta_step[0];
	Theta[1] = Theta[1] + Theta_step[1];
	Theta[2] = Theta[2] + Theta_step[2];
};


