#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "transform3d.h"
#include "model.h"

class Primitive {
public:
	/* ��Ԫ���� */
	int type;                     // ��Ԫ����
	vector<float> paras;          // ��Ԫ����
	int point_num;                // ����
	vector<unsigned int> indices; // ���� Shape �е�����

	Mesh *model_pointclouds;      // ����ģ��

	glm::vec3 center;

	/* �������� */
	Model *model_shape;           // ��״ģ��
	bool selected;                // �Ƿ�ѡ��
	vector<Vertex> points_gt;        // ��ʵ���ݣ��������
	vector<unsigned int> indices_gt; // ��ʵ���ݣ��������
	Mesh *model_gt;                  // ��ʵ���ݣ�ģ��

	// Ĭ�Ϲ���
	Primitive() : type(-1), point_num(0), model_pointclouds(nullptr), model_shape(nullptr), selected(false) { }
	Primitive(string modelpath) : type(-1), point_num(0), model_pointclouds(nullptr), selected(false) {
		model_shape = new Model(modelpath);
	}

	// ���Ա任
	void inline rotate(float _angle, glm::vec3 _axis) { model_shape->rotate(_angle, _axis); }                 // ��ת
	void inline rotate(float _angle, float _x, float _y, float _z) { rotate(_angle, glm::vec3(_x, _y, _z)); } // ��ת
	void inline rotate(glm::vec3 _src, glm::vec3 _dst) { model_shape->rotate(_src, _dst); }                   // ��ת
	void inline rotate(glm::vec3 _src, float _x, float _y, float _z) { rotate(_src, glm::vec3(_x, _y, _z)); } // ��ת
	void inline translate(glm::vec3 _dxyz) { model_shape->translate(_dxyz); }                                 // ƽ��
	void inline translate(float _x, float _y, float _z) { translate(glm::vec3(_x, _y, _z)); }                 // ƽ��
	void inline scale(glm::vec3 _dxyz) { model_shape->scale(_dxyz); }                                         // ����
	void inline scale(float _x, float _y, float _z) { scale(glm::vec3(_x, _y, _z)); }                         // ����

	void createModel() {
		switch (type) {
		case 1:
			model_shape = new Model("shapes/plane_face.obj");
			rotate(glm::vec3(0.0f, 1.0f, 0.0f), paras[0], paras[1], paras[2]);
			translate(glm::vec3(0.0f, paras[3], 0.0f));
			break;
		case 2:
			model_shape = new Model("shapes/cylinder_face_x.obj");
			translate(paras[0], paras[1], paras[2]);
			rotate(glm::vec3(0.0f, 1.0f, 0.0f), paras[3], paras[4], paras[5]);
			scale(paras[6], paras[6], paras[6]);
			break;
		case 3:
			model_shape = new Model("shapes/cone_face.obj");
			translate(paras[0], paras[1], paras[2]);
			rotate(glm::vec3(0.0f, -1.0f, 0.0f), paras[3], paras[4], paras[5]);
			scale(glm::vec3(1.0f, 1.0f/std::tanf(paras[6])/2.0f, 1.0f));
			break;
		default:
			break;
		}
	}
};

#endif  // PRIMITIVE_H