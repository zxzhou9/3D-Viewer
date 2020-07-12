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
	vector<Primitive> instances; // ��״�б�
	vector<Vertex> points;       // ����ȫ���������
	vector<unsigned int> indices; // ����ȫ���������
	vector<Vertex> points_highlight; // һ����״�ĵ㼯����

	Mesh *raw_points; // ����ȫ����� mesh��������ʾ��
	
	Shape(string points_path, string paras_path) {
		importPoints(points_path, glm::vec3(0.5f,0.5f,0.5f));
		importParas(paras_path, glm::vec3(1.0f, 0.0f, 0.0f));
	}

private:
	// �������ݵ�ȫ����
	void importPoints(string points_path, glm::vec3 _color = glm::vec3(0.8f, 0.8f, 0.8f)) {
		fstream file(points_path);
		vector<string> lines;
		string line;
		stringstream s;
		float x, y, z;
		
		// ����㼯�ļ�
		while (getline(file, line)) {
			lines.push_back(line);
		}

		// ����ÿ���� xyz
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

	// ������״��instance���Ĳ�����һ�����ݺ��ж����״
	void importParas(string paras_path, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f)) {
		fstream paras_file(paras_path);
		vector<string> lines;
		string line;
		stringstream s;
		float x, y, z;
		bool dirty = false;
		int pre = -1;
		int m;

		// ��������ļ�
		while (getline(paras_file, line)) {
			lines.push_back(line);
		}

		// �������ݵ�ÿ����״
		for (unsigned int i = 0; i < lines.size(); i++) {
			// ÿ����״�ԡ�----����ͷ
			if (lines[i].c_str()[3] == '-') {
				Primitive ins;

				// ��״���ͣ�1 ƽ�� 2 Բ�� 3 Բ׶
				i++;
				ins.type = atoi(lines[i].substr(2).c_str());

				/* ������״�Ĳ��� */
				i++;
				// Բ׶
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
				// Բ��
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
				// ƽ��
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

				// ��״�ĵ���
				i++;
				ins.point_num = atoi(lines[i].c_str());

				// ��״�ĵ㼯�����б�
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

				// ������״�� mesh��������ʾ��
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