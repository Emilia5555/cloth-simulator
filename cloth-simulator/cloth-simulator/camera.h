#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	// 3D position of the camera
	glm::vec3 position;
	// what the camera is looking at
	glm::vec3 target;
	// which direction is up for the camera
	glm::vec3 up;
	// field of view
	float fov;
	// width / height of your window
	float aspectRatio;
	// closest distance camera can see
	float nearPlane;
	// farthest distance camera can see
	float farPlane;

	// default constuctor
	Camera() {
		position = glm::vec3(0.0f, 0.0f, 3.0f);
		target = glm::vec3(0.0f, 0.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		fov = 45.0f;
		aspectRatio = 1.0f;
		nearPlane = 0.1f;
		farPlane = 100.0f;
	}

	// constructor
	Camera(glm::vec3 pos, glm::vec3 trgt, glm::vec3 upDir, float iFov, float iAspectRatio, float iNearPlane, float iFarPlane)
	{
		position = pos;
		target = trgt;
		up = upDir;
		fov = iFov;
		aspectRatio = iAspectRatio;
		nearPlane = iNearPlane;
		farPlane = iFarPlane;
	}

	// getters
	// calculates and returns the view matrix based on the cameras position, target, and up direction
	glm::mat4 getViewMatrix() const 
	{
		// lookAt() returns a 4x4 a view matrix
		return glm::lookAt(position, target, up); 
	}

	// calculates and returns the perspective projection matrix
	glm::mat4 getProjectionMatrix() const 
	{
		// perspective() returns a 4x4 perspective projection matrix
		return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	}

};

