#ifndef MESH_H
#define MESH_H

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>


// opengl & glfw
#include <GL/gl3w.h>


//#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
//#include <GL/gl3w.h>    // Initialize with gl3wInit()
//#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
//#include <GL/glew.h>    // Initialize with glewInit()
//#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
//#include <glad/glad.h>  // Initialize with gladLoadGL()
//#else
//#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
//#endif
//
//// Include glfw3.h after our OpenGL definitions
//#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;


// ����
struct Vertex {
	glm::vec3 position;   // λ��
	glm::vec3 color;      // ��ɫ
	glm::vec3 normal;     // ������
	//glm::vec2 texCoords;  // ��������

	Vertex(glm::vec3 _position, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f)) {
		this->position = _position;
		this->color = _color;
		this->normal = glm::vec3(0.0f, 0.0f, 0.0f);
		//this->texCoords = glm::vec2(0.0f, 0.0f);
	}
};

// ����
//struct Texture {
//	unsigned int id;  // ����id
//	string type;      // �������ͣ�diffuse�������䣩��specular�����淴�䣩��
//	string path;      // �����ļ�·��
//};

class Mesh {
public:
	/*  ��������  */
	vector<Vertex> m_vertices;       // �����б�
	vector<unsigned int> m_indices;  // �����б�
	//vector<Texture> m_textures;      // �����б�
	unsigned int m_VAO;              // �����������
private:
	glm::mat4 model_matrix;
public:
	glm::mat4& getModelMatrix() {
		return model_matrix;
	}
	/*  ����  */
	// ���캯��
	Mesh(vector<Vertex> _vertices, vector<unsigned int> _indices)
	{
		model_matrix = glm::mat4(1.0f);
		this->m_vertices = _vertices;
		this->m_indices = _indices;
		//this->m_textures = _textures;
		// ��ʼ�����������������ָ��
		setupMesh();
	}

	// ��ת
	void rotate(float angle, glm::vec3 axis) {
		model_matrix = glm::rotate(model_matrix, angle, axis);
	}
	// λ��
	void translate(glm::vec3 dxyz) {
		model_matrix = glm::translate(model_matrix, dxyz);
	}
	// ����
	void scale(glm::vec3 dxyz) {
		model_matrix = glm::scale(model_matrix, dxyz);
	}

	// ��Ⱦ����
	void draw(Shader *shader, int drawType)
	{
		shader->use();

		// ������
		//unsigned int diffuseNr = 1;
		//unsigned int specularNr = 1;
		//unsigned int normalNr = 1;
		//unsigned int heightNr = 1;
		//for (unsigned int i = 0; i < m_textures.size(); i++)
		//{
		//	glActiveTexture(GL_TEXTURE0 + i); // ������ǰ�������Ӧ������Ԫ

		//	/*  ƴ������id�����磺diffuse_texture + N = diffuse_textureN  */
		//	// �����ת���ַ���
		//	string number;
		//	string name = m_textures[i].type;
		//	if (name == "texture_diffuse")
		//		number = std::to_string(diffuseNr++);
		//	else if (name == "texture_specular")
		//		number = std::to_string(specularNr++);
		//	else if (name == "texture_normal")
		//		number = std::to_string(normalNr++);
		//	else if (name == "texture_height")
		//		number = std::to_string(heightNr++);
		//	// ������Ԫ������ɫ���ж�Ӧ�Ĳ�����
		//	glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
		//	//cout << "���������: " << name << number << endl;
		//	// ������Ԫ
		//	glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		//}

		// ��������

		glBindVertexArray(m_VAO);
		if (drawType == 0)
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		else if (drawType == 1)
			glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);
		else if (drawType == 2)
			glDrawElements(GL_POINTS, m_indices.size(), GL_UNSIGNED_INT, 0);
		//cout << m_indices.size() << endl;
		glBindVertexArray(0);

		// �������Ժ��л���Ĭ������
		//glActiveTexture(GL_TEXTURE0);
	}


private:
	/*  ��Ⱦ����  */
	unsigned int m_VBO;  // ���㻺�����
	unsigned int m_EBO;  // �����������

	/*  ����    */
	// ��ʼ�����ֻ������
	void setupMesh()
	{
		// ���ɻ������
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);
		// �������ݵ�VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

		// �������ݵ�EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

		// ���ö�������ָ��
		// ����λ��
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// ������ɫ
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// ���㷨����
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// ������������
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		// �л���Ĭ��VAO
		glBindVertexArray(0);
	}
};
#endif  // MESH_H