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
	// angle between 0 and z axis
	float yaw;
	// angle between 0 and y axis
	float pitch;
	// how far the camera is from the target
	float distance;
	// tracks if left mouse button is pressed down
	bool isDragging;
	// mouseX from the previous frame
	double lastMouseX;
	// mouseY from the previous frame
	double lastMouseY;


	// default constuctor
	Camera() {
		position = glm::vec3(0.0f, 0.0f, 3.0f);
		target = glm::vec3(0.0f, 0.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		fov = 45.0f;
		aspectRatio = 1.0f;
		nearPlane = 0.1f;
		farPlane = 100.0f;

		yaw = 0.0f;
		pitch = 0.0f;
		distance = 3.0f;
		isDragging = false;
		lastMouseX = 0.0f;
		lastMouseY = 0.0f;

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

		yaw = 0.0f;
		pitch = 0.0f;
		distance = 3.0f;
		isDragging = false;
		lastMouseX = 0.0f;
		lastMouseY = 0.0f;
		
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

	//updates position of camera
	void updateOrbit() {
		// start at target because we need the distance from what the camera is looking at
		position.x = target.x + distance * cos(pitch) * sin(yaw);
		position.y = target.y + distance * sin(pitch);
		position.z = target.z + distance * cos(pitch) * cos(yaw);

	}

	// resets camera to default values
	void reset() {
		yaw = 0.0f;
		pitch = 0.0f;
		distance = 3.0f;
		isDragging = false;
		lastMouseX = 0.0f;
		lastMouseY = 0.0f;
		updateOrbit();
	}
};

// Called by GLFW when a mouse button is pressed or released
// sets isDragging to positive when left mouse is clicked
// window: GLFW window that recived the event
// button: which mouse button triggered the event
// action: GLFW_PRESS or GFLW_RELEASE
// extra int parameter to match GLFW
void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
	// prevents function from running if mouse is hovering over ImGui panel
	if (ImGui::GetIO().WantCaptureMouse) {
		// call ImGui callbacks
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
		// exits function
		return;
	}
	
	// glfw window can a void pointer that can point to anything
	// cam stores a pointer to my camera object in main
	// i get that pointer from the second command and cast it as a pointer to a Camera
	Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	// checks if the left mouse button was affected
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		// chekcs if that button was pressed down
		if (action == GLFW_PRESS) {
			// sets is dragging to true
			cam->isDragging = true;
			//stores the position of the cursor 
			glfwGetCursorPos(window, &cam->lastMouseX,&cam->lastMouseY);
		}
		// checks if that button was released
		else if (action == GLFW_RELEASE) {
			// sets is dragging to true
			cam->isDragging = false;
		}
	}
}

// Called by GLFW everytime the mouse moves
// if left mouse button is pressed, updates yaw pitch and camera position
// window: GLFW window that recived the event
// xpos: current cursor x position in pixels
// ypos: current cursor y position in pixels
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	// prevents function from running if mouse is hovering over ImGui panel
	if (ImGui::GetIO().WantCaptureMouse) {
		// call ImGui callbacks
		ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
		// exits function
		return;
	}

	// points to our camera object in main
	Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	// if the left mouse button is pushed down
	if (cam->isDragging) {
		// slows down the camera movement
		float sensetivity = 0.005f;
		// calculates how far the mouse has moved since last frame
		float xDelta = static_cast<float>(xpos - cam->lastMouseX);
		float yDelta = static_cast<float>(ypos - cam->lastMouseY);

		// subtract the amount of movement in the x direction to yaw
		cam->yaw -= xDelta * sensetivity;
		// adds movement in y direction from pitch
		cam->pitch += yDelta * sensetivity;

		// caps the camera at just below 90° above or below the target
		// full range is pi/2 to -pi/2 ~= 1.5 to -1.5
		if (cam->pitch < -1.5f) {
			cam->pitch = -1.5f;
		} 
		if (cam->pitch > 1.5f) {
			cam->pitch = 1.5f;
		}

		// set lastmouseX to most recent x position reading
		cam->lastMouseX = xpos;
		// set lastmouseY to most recent y position reading
		cam->lastMouseY = ypos;
		// update camera position
		cam->updateOrbit();
	}
}

// called by GLFW when scrollbarr moves
// adjusts camera distance to zoom in or out
// window: GLFW window that recived the event
// xoffset: unused only here to match GLFW
// yoffset: how much the scrollbar has moved vertically (up +, down -)
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	// prevents function from running if mouse is hovering over ImGui panel
	if (ImGui::GetIO().WantCaptureMouse) {
		// call ImGui callbacks
		ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
		// exits function
		return;
	}
	
	// points to our camera object in main
	Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	// subtracts y offset from distance
	// 0.2f dampens speed
	// subtract because scrolling down is negative (zoom out)
	// yoffset is delta of scrollbar
	cam->distance -= static_cast<float>(yoffset * 0.2f);

	// cap distance at reasonable values
	if (cam->distance < 0.5f) {
		cam->distance = 0.5f;
	}
	if (cam->distance > 20.0f) {
		cam->distance = 20.0f;
	}
	// update camera position
	cam->updateOrbit();
}