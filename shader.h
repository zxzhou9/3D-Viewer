#ifndef SHADER_H
#define SHADER_H

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>


// opengl & glfw
#include <GL/gl3w.h>

//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
//
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

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID; // ��ɫ��id
	// ������ɫ��������
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	{
		/*  1. ���ļ��ж�ȡ��ɫ������  */
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ȷ��ifstream��������׳��쳣��
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			/*  ���ļ��ж�ȡGLSL��ɫ������  */
			// ������ɫ��
			vShaderFile.open(vertexPath);
			std::stringstream vShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			vShaderFile.close();
			vertexCode = vShaderStream.str();

			// Ƭ����ɫ��
			fShaderFile.open(fragmentPath);
			std::stringstream fShaderStream;
			fShaderStream << fShaderFile.rdbuf();
			fShaderFile.close();
			fragmentCode = fShaderStream.str();

			// ������ɫ��
			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure e)
		{   // ���������Ϣ
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		/*  2. ������ɫ��  */
		// ������ɫ��
		unsigned int vertex;
		const char* vShaderCode = vertexCode.c_str();
		vertex = glCreateShader(GL_VERTEX_SHADER);     // ������ɫ��
		glShaderSource(vertex, 1, &vShaderCode, NULL); // ����ɫ��Դ��
		glCompileShader(vertex);                       // ������ɫ��
		checkCompileErrors(vertex, "VERTEX");          // �����ɫ���������

		// Ƭ����ɫ��
		unsigned int fragment;
		const char * fShaderCode = fragmentCode.c_str();
		fragment = glCreateShader(GL_FRAGMENT_SHADER);   // ������ɫ��
		glShaderSource(fragment, 1, &fShaderCode, NULL); // ����ɫ��Դ��
		glCompileShader(fragment);                       // ������ɫ��
		checkCompileErrors(fragment, "FRAGMENT");        // �����ɫ���������

		// ������ɫ��
		unsigned int geometry;
		if (geometryPath != nullptr)
		{
			const char * gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);   // ������ɫ��
			glShaderSource(geometry, 1, &gShaderCode, NULL); // ����ɫ��Դ��
			glCompileShader(geometry);                       // ������ɫ��
			checkCompileErrors(geometry, "GEOMETRY");        // �����ɫ���������
		}

		// ��ɫ������
		ID = glCreateProgram();            // ������ɫ������
		glAttachShader(ID, vertex);        // �󶨶�����ɫ��
		glAttachShader(ID, fragment);      // ��Ƭ����ɫ��
		if (geometryPath != nullptr)
			glAttachShader(ID, geometry);  // �󶨼�����ɫ��
		glLinkProgram(ID);                 // ������ɫ������
		checkCompileErrors(ID, "PROGRAM"); // �����ɫ������������

		// ��ɫ���������ӳɹ����ͷ���ɫ����Դ
		glDeleteShader(vertex);       // �ͷŶ�����ɫ��
		glDeleteShader(fragment);     // �ͷ�Ƭ����ɫ��
		if (geometryPath != nullptr)
			glDeleteShader(geometry); // �ͷż�����ɫ��
	}

	// ������ɫ��
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}

	// ���ߺ���������uniformȫ�ֱ���
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value)
	{
		use();
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value)
	{
		use();
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value)
	{
		use();
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value)
	{
		use();
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y)
	{
		use();
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value)
	{
		use();
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z)
	{
		use();
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value)
	{
		use();
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		use();
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat)
	{
		use();
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat)
	{
		use();
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat)
	{
		use();
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	// ���ߺ�������ɫ��������
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{   // ��ɫ��������
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				// ���������Ϣ
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
					<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{   // ��ɫ�����
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				// ���������Ϣ
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
					<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif  // SHADER_H