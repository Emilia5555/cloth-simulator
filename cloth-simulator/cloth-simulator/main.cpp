// lets me access OpenGL functions
#include <glad/glad.h>
// lets me create window and use GLFW functions
#include <GLFW/glfw3.h>
#include <iostream>
// incude shaders and shader functions
#include "shader_utils.h"
#include "shaders.h"
#include <vector>
#include <cmath>
// particle struct
#include "particle.h"
// physics
#include "physics.h"


int main() {

	// if window doesnt initialize correctly, print error and exit
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// initialize some stuff about the window before i create it
	// tell GLFW to open a window with version 3.3 (major.minor) 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// tell GLFW to use the core profile, which means we only have access to modern OpenGL functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creates the window (width px, height px, title, monitor for fullscreen, share context)
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "Cloth Simulator", nullptr, nullptr);
	// if window doesn't get created, print error, terminate window, and exit
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		// avoid memory leak by terminating GLFW before exiting
		glfwTerminate();
		return -1;
	}

	//applies the OpenGL context to the window. essentially just makes OpenGL work with the window we just made
	glfwMakeContextCurrent(window);

	//initialize GLAD to make GLFW work with windows
	//use glfwGetProcAdress from GLFW to show GLAD where to get the functions we need 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialze GLAD" << std::endl;
		return -1;
	}

	// create shader program object for vertex and fragment shader
	unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	// 10x 10 grid of particle
	const int GRID_W = 10;
	const int GRID_H = 10;

	std::vector<Particle> particles;

	// loop through height and width
	for (int y = 0; y < GRID_H; y++) {
		for (int x = 0; x < GRID_W; x++) {
			// space points evenly across the grid
			// *2 -1 changes range of point from 0-1 to 0-2 to -1 to 1
			// make divisor a float to avoid integer division
			float px = (x / (float)(GRID_W - 1) * 1.8f - 0.9f);
			float py = (y / (float)(GRID_H - 1) * 1.8f - 0.9f);

			// make a particle variable 
			Particle p;
			//set position to the variables we calculated earlier
			p.position = glm::vec3(px, py, 0.0f);
			// set velocity to 0
			p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			p.pinned = false;
			// add to vector of Particles
			particles.push_back(p);
		
		
		}
	}

	// holds ID for vertex array object
	unsigned int VAO;
	// holds ID for vertex buffer object
	unsigned int VBO;
	// generates ID for vertex array objects and stores in VAO
	glGenVertexArrays(1, &VAO);
	// generates ID for vertex array objects and stores in VBO
	glGenBuffers(1, &VBO);
	// makes our VAO currently active
	glBindVertexArray(VAO);

	// makes VBO the current active GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// allocates space for particles in the GPU
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(float) * 3, nullptr, GL_DYNAMIC_DRAW);

	

	// (void*)0 means start reading at byte 0 of the buffer
	//tells open gl that VBO has vertex data where each vertex is three floats and the vertexes are three floats apart
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// enables input slot 0
	glEnableVertexAttribArray(0);


	// unbind VAO and VBO
	// binding 0 is unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//time tracking
	float lastTime = glfwGetTime();

	// while the user has not closed the window 
	while (!glfwWindowShouldClose(window)) {
		//deltaTime tracking
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		applyForces(particles, deltaTime);


		// creates a vector of floats from the particle positions
		std::vector<float> positionData;
		for (const Particle& p : particles) {
			positionData.push_back(p.position.x);
			positionData.push_back(p.position.y);
			positionData.push_back(p.position.z);
		};

		// bind VBO before using
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// updates the position data 
		glBufferSubData(GL_ARRAY_BUFFER, 0, positionData.size() * sizeof(float), positionData.data());

		// indicates color to set (red,green, blue, alpha) uses 0-1 scale, expects float values
		glClearColor(1.0f, 0.627f, 0.992f, 0.55f);
		// sets the color 
		glClear(GL_COLOR_BUFFER_BIT);
		// makes the draw call use the correct shader program
		glUseProgram(shaderProgram);
		// bind VAO before drawing
		glBindVertexArray(VAO);
		// makes the points 10.f
		glPointSize(10.0f);
		// draws the points (kind of point, index of first point, point count)
		glDrawArrays(GL_POINTS, 0, GRID_H * GRID_W);


		// prevents screen from flickering by drawing to a back buffer and then swapping it to the front
		glfwSwapBuffers(window);
		// detects input events, without this line the program wouldnt respond to any input
		glfwPollEvents();
	}

	// cleanup
	glfwTerminate();
	return 0;
}