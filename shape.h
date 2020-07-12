#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "primitive.h"
#include "cone.h"
#include "cylinder.h"
#include "plane.h"
#include <Windows.h>
#include "transform.h"

struct SPFN_GTInfo {
	int id;
	string type;
	float para1;
	float para2;
	float para3;
};

class Shape {
public:
	vector<Primitive> instances; // 形状列表
	vector<Vertex> points;       // 数据全部点的坐标
	vector<unsigned int> indices; // 数据全部点的索引
	vector<Vertex> points_highlight; // 一个形状的点集高亮

	Mesh *raw_points; // 数据全部点的 mesh（用于显示）
	
	Shape(string points_path, string paras_path) {
		importPoints(points_path, glm::vec3(0.5f,0.5f,0.5f));
		importParas(paras_path, glm::vec3(1.0f, 0.0f, 0.0f));
	}

private:
	// 读入数据的全部点
	void importPoints(string points_path, glm::vec3 _color = glm::vec3(0.8f, 0.8f, 0.8f)) {
		fstream file(points_path);
		vector<string> lines;
		string line;
		stringstream s;
		float x, y, z;
		
		// 读入点集文件
		while (getline(file, line)) {
			lines.push_back(line);
		}

		// 读入每个点 xyz
		for (unsigned int i = 0; i < lines.size(); i++) {
			s.str("");
			s.clear();
			s << lines[i];
			s >> x >> y >> z;
			points.push_back(Vertex(glm::vec3(x, y, z), _color));
			points_highlight.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(1.0f, 0.0f, 0.0f)));
			indices.push_back(i);
		}

		raw_points = new Mesh(points, indices);
	}

	// 读入形状（instance）的参数，一个数据含有多个形状
	void importParas(string paras_path, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f)) {
		fstream paras_file(paras_path);
		vector<string> lines;
		string line;
		stringstream s;
		float x, y, z;
		bool dirty = false;
		int pre = -1;
		int m;

		// 读入参数文件
		while (getline(paras_file, line)) {
			lines.push_back(line);
		}

		// 读入数据的每个形状
		for (unsigned int i = 0; i < lines.size(); i++) {
			// 每个形状以“----”开头
			if (lines[i].c_str()[3] == '-') {
				Primitive ins;

				// 形状类型，1 平面 2 圆柱 3 圆锥
				i++;
				ins.type = atoi(lines[i].substr(2).c_str());

				/* 读入形状的参数 */
				i++;
				// 圆锥
				if (ins.type == 3) {
					s.str("");
					s.clear();
					s << lines[i];
					s >> x >> y >> z;
					ins.paras.push_back(x); // apex
					ins.paras.push_back(y);
					ins.paras.push_back(z);
					ins.center = glm::vec3(x, y, z);
					i++;
					s.str("");
					s.clear();
					s << lines[i];
					s >> x >> y >> z;
					ins.paras.push_back(x); // axis
					ins.paras.push_back(y);
					ins.paras.push_back(z);
					i++;
					s.str("");
					s.clear();
					s << lines[i];
					s >> x;
					ins.paras.push_back(x); // half angle
				}
				// 圆柱
				else if (ins.type == 2) {
					s.str("");
					s.clear();
					s << lines[i];
					s >> x >> y >> z;
					ins.paras.push_back(x); // center
					ins.paras.push_back(y);
					ins.paras.push_back(z);
					ins.center = glm::vec3(x, y, z);
					i++;
					s.str("");
					s.clear();
					s << lines[i];
					s >> x >> y >> z;
					ins.paras.push_back(x); // axis
					ins.paras.push_back(y);
					ins.paras.push_back(z);
					i++;
					s.str("");
					s.clear();
					s << lines[i];
					s >> x;
					ins.paras.push_back(x); // radius
				}
				// 平面
				else if (ins.type == 1) {
					s.str("");
					s.clear();
					s << lines[i];
					s >> x >> y >> z;
					ins.paras.push_back(x); // n
					ins.paras.push_back(y);
					ins.paras.push_back(z);
					ins.center = glm::vec3(x, y, z);
					i++;
					s.str("");
					s.clear();
					s << lines[i];
					s >> x;
					ins.paras.push_back(x); // c
					ins.center *= x;
				}

				// 形状的点数
				i++;
				ins.point_num = atoi(lines[i].c_str());

				// 形状的点集索引列表
				if (ins.point_num > 0) {
					i++;
					s.str("");
					s.clear();
					s << lines[i];
					for (unsigned int pp = 0; pp < ins.point_num; pp++) {
						s >> m;
						ins.indices.push_back(m);	
					}

				}

				// 创建形状的 mesh（用于显示）
				ins.createModel();
				if (ins.point_num > 0) {
					ins.model_pointclouds = new Mesh(points_highlight, ins.indices);
				}
				else {
					ins.model_pointclouds = nullptr;
				}
				instances.push_back(ins);
			}
		}
	}

};

#endif  // SHAPE_H