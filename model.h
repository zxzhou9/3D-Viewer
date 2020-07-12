#ifndef MODEL_H
#define MODEL_H

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>


// opengl & glfw
#include <GL/gl3w.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <ctime>

using namespace std;

//unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
public:
	/*  ģ������  */
	//vector<Texture> textures_loaded; // ���������Ѽ��ص�����
	vector<Mesh> meshes; // �����ģ������������
	//string directory; // ģ������Ŀ¼
	//bool gammaCorrection; // ٤�����
private:
	glm::mat4 model_matrix;

public:
	glm::mat4& getModelMatrix() {
		return model_matrix;
	}
	/*  ����  */
	// ���캯�������ļ�����ģ��
	Model(string const &path) {
		model_matrix = glm::mat4(1.0f);
		loadModel(path); // ���ļ�����ģ��
	}


	// ����ģ�ͣ������Ƹ�ģ���µ���������
	void draw(Shader *shader, int drawType = 0) {
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].draw(shader, drawType);
	}

	// ��ת����, �ǣ�
	void inline rotate(float angle, glm::vec3 axis) {
		model_matrix = glm::rotate(model_matrix, angle, axis);
	}
	// ��ת���� -> �ᣩ
	void inline rotate(glm::vec3 src, glm::vec3 dst) {
		src = glm::normalize(src);
		dst = glm::normalize(dst);
		float dot = src.x*dst.x + src.y*dst.y + src.z*dst.z;
		float angle = std::acosf(dot);
		glm::vec3 axis = glm::normalize(glm::cross(src, dst));
		rotate(angle, axis);
	}
	// λ��
	void translate(glm::vec3 dxyz) {
		model_matrix = glm::translate(model_matrix, dxyz);
	}
	// ����
	void scale(glm::vec3 dxyz) {
		model_matrix = glm::scale(model_matrix, dxyz);
	}

private:
	/* ���� */
	// ģ���ļ� -> Assimp -> Model/Mesh
	void loadModel(string const &path)
	{
		// ���ļ��ж�ȡģ��
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_GenNormals);

		// ������
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // ���������0
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// ����ģ������Ŀ¼
		//directory = path.substr(0, path.find_last_of('/'));

		// ��Assimp���ڵ㿪ʼ���ݹ鴦�����нڵ�
		processNode(scene->mRootNode, scene);
	}

	// ������aiNode�µ�����aiMesh
	void processNode(aiNode *node, const aiScene *scene)
	{
		// ����ǰaiNode�µ�����aiMesh
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// aiNodeֻ�������������������ݶ�������aiScene
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // �ҵ�aiMesh��aiScene�еľ���λ��
			meshes.push_back(processMesh(mesh, scene));      // ����תΪ�Զ���Mesh�࣬�����Զ���Model����
		}
		// �ݹ���ñ���������ǰaiNode���ӽڵ�
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	// ������aiMesh
	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// ����Mesh��������������
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		//vector<Texture> textures;

		// ������ǰmesh���ж���
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex(glm::vec3(0.0f, 0.0f, 0.0f));
			glm::vec3 vector; // ��Assimp�Ķ���ת��Ϊglm::vec3
			glm::vec2 vec;

			// λ��
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			// ��ɫ
			if (mesh->HasVertexColors(0))
			{
				vector.x = mesh->mColors[0][i].r;
				vector.y = mesh->mColors[0][i].g;
				vector.z = mesh->mColors[0][i].b;
			}
			else
			{
				vertex.color = glm::vec3(0.5f, 0.5f, 0.5f);
			}

			// ������
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x ;
				vector.y = mesh->mNormals[i].y ;
				vector.z = mesh->mNormals[i].z ;
				vertex.normal = vector;
			}

			// ��������
			//if (mesh->HasTextureCoords(0)) // �����������
			//{
			//	// һ��������������8���������꣬����ֻȡ��һ��
			//	vec.x = mesh->mTextureCoords[0][i].x;
			//	vec.y = mesh->mTextureCoords[0][i].y;
			//	vertex.texCoords = vec;
			//}

			vertices.push_back(vertex);
		}

		// ������ǰmesh������������
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// ����������������ж�������
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// �������
		//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// ����������ͼ������ʽ��
		// diffuse:  texture_diffuseN
		// specular: texture_specularN
		// normal:   texture_normalN

		// 1. diffuse maps
		//vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		//vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// ����һ���Զ���Mesh�����
		return Mesh(vertices, indices);
	}

	// ����ĳһ���Ͳ�����ͼ�������û�м��أ�
	//vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	//{
	//	vector<Texture> textures;
	//	// �������и����Ͳ�����ͼ
	//	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	//	{
	//		aiString str;
	//		mat->GetTexture(type, i, &str);
	//		// �������Ƿ��Ѽ��أ�����Ѽ���������
	//		bool skip = false;
	//		for (unsigned int j = 0; j < textures_loaded.size(); j++)
	//		{
	//			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
	//			{
	//				textures.push_back(textures_loaded[j]);
	//				skip = true; // ��ǰ·�������Ѽ��أ�����
	//				break;
	//			}
	//		}
	//		if (!skip)
	//		{   // �����ǰ·�����ʻ�û���ع�����ô������
	//			Texture texture;
	//			texture.id = TextureFromFile(str.C_Str(), this->directory);
	//			texture.type = typeName;
	//			texture.path = str.C_Str();
	//			textures.push_back(texture);
	//			textures_loaded.push_back(texture);  // ���صĲ��ʱ�����Model�У���ֻ֤����һ�Σ����ظ�����
	//			cout << "�Ѽ�����ͼ: " << texture.id << " ·��: " << texture.path << " ����: " << texture.type << endl;
	//		}
	//	}
	//	return textures;
	//}
};


// ���ļ��ж�ȡ������ͼ
//unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
//{
//	// ƴ���ļ�·��
//	string filename = string(path);
//	filename = directory + '/' + filename;
//
//	// �����������
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	// ��ʼ���ļ���ȡ��ʹ��stb_image�⣩
//	int width, height, nrComponents;
//	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		// �ж���ɫͨ����
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		// ���������
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		// �����ƺ͹��˷�ʽ
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		// �ͷ���Դ
//		stbi_image_free(data);
//	}
//	else
//	{   // ��ȡʧ��
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureID; // ��������id
//}
#endif  // MODEL_H