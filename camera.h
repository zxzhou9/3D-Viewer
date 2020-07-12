#ifndef CAMERA_H
#define CAMERA_H

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>


// opengl & glfw
#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
using namespace glm;

#include <vector>

constexpr auto PI = 3.141592653;

// �������
const float SENSITIVITY = 0.005f;
const float ZOOM = 25.0f;

class Camera {
public:
	// �������
	glm::vec3 Position; // ���λ��
	glm::vec3 Front;    // �������
	glm::vec3 Up;       // �������
	glm::vec3 Right;    // �������
	glm::vec3 WorldUp;  // ����������
	glm::vec3 Target;   // �۲��

	float r;

	// ���ѡ��
	float MouseSensitivity; // ���������
	float Zoom;             // �������

	// ���������ͨ��������
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 30.0f), // ���λ��
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))           // ����������
		: MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Target = glm::vec3(0.0f, 0.0f, 0.0f);
		r = sqrtf(Position.x * Position.x + Position.y * Position.y + Position.z * Position.z);
		Front = glm::normalize(Target - Position);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}


	// ��ȡ view ����
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}


	// ����ƶ��¼����������ı�����Ƕȣ�
	void MouseMovementMeshLabStyle(float xoffset, float yoffset)
	{
		xoffset *= MouseSensitivity; // ��� x ��ƫ�������Ƕȣ�
		yoffset *= MouseSensitivity; // ��� y ��ƫ�������Ƕȣ�

		glm::quat rotateQuat;
		glm::mat4 transMatrix;

		rotateQuat = glm::angleAxis(-xoffset, Up);
		transMatrix = glm::toMat4(rotateQuat);
		Position = vec3(transMatrix * vec4(Position, 1.0f));
		Front = glm::normalize(Target - Position);
		Right = glm::normalize(glm::cross(Front, Up));

		rotateQuat = glm::angleAxis(yoffset, Right);
		transMatrix = glm::toMat4(rotateQuat);
		Position = vec3(transMatrix * vec4(Position, 1.0f));
		Front = glm::normalize(Target - Position);
		Up = glm::normalize(glm::cross(Right, Front));
	}

	// ����ƶ��¼����������ı�����Ƕȣ�
	void MouseMovementPlatformStyle(float xoffset, float yoffset)
	{
		xoffset *= MouseSensitivity; // ��� x ��ƫ�������Ƕȣ�
		yoffset *= MouseSensitivity; // ��� y ��ƫ�������Ƕȣ�

		glm::quat rotateQuat;
		glm::mat4 transMatrix;

		rotateQuat = glm::angleAxis(-xoffset, WorldUp);
		transMatrix = glm::toMat4(rotateQuat);
		Position = vec3(transMatrix * vec4(Position, 1.0f));
		Front = glm::normalize(Target - Position);
		Up = vec3(transMatrix * vec4(Up, 1.0f));
		Right = glm::normalize(glm::cross(Front, Up));

		rotateQuat = glm::angleAxis(yoffset, Right);
		transMatrix = glm::toMat4(rotateQuat);
		Position = vec3(transMatrix * vec4(Position, 1.0f));
		Front = glm::normalize(Target - Position);
		Up = glm::normalize(glm::cross(Right, Front));
	}


	void translate(float xoffset, float yoffset)
	{
		xoffset *= MouseSensitivity; // ��� x ��ƫ�������Ƕȣ�
		yoffset *= MouseSensitivity; // ��� y ��ƫ�������Ƕȣ�

		/*Position += Right * (-xoffset);
		Position += Up * (-yoffset);
		Target += Right * (-xoffset);
		Target += Up * (-yoffset);*/

		Position += Right * (-xoffset);
		Position += Up * (-yoffset);
		Target += Right * (-xoffset);
		Target += Up * (-yoffset);
	}


	// �������¼���Ӧ�������ı������۲����룩
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 0.1f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 0.1f)
			Zoom = 0.1f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}


	// �������
	void resetCamera()
	{
		Position = glm::vec3(0.0f, 0.0f, 30.0f);
		WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		Target = glm::vec3(0.0f, 0.0f, 0.0f);
		r = sqrtf(Position.x * Position.x + Position.y * Position.y + Position.z * Position.z);
		Zoom = ZOOM;
		MouseSensitivity = SENSITIVITY;

		Front = glm::normalize(Target - Position);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif // CAMERA_H