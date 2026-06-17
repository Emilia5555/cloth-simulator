#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;
};

