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
// physics and forces
#include "physics.h"
// spring
#include "spring.h"

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



	// generate particles
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
			p.previousPosition = p.position;
			// add to vector of Particles
			particles.push_back(p);

			// pin top left corrner
			if (x == 0 && y == GRID_H - 1) {
				particles.back().pinned = true;
			}
			 //pin top right corrner
			if (x == GRID_W -1 && y == GRID_H - 1) {
				particles.back().pinned = true;
			}
		
		}
	}

	//generate springs
	std::vector <Spring> springs;
	float stiffness = 0.00088f;
	float sheerStiffness = 0.00044f;
	float bendStiffness = 0.0001f;

	for (int y = 0; y < GRID_H; y++) {
		for (int x = 0; x < GRID_W; x++) {
			//converts 2D grid coordinatesinto a 1D vector index 
			int index = y * GRID_W + x;

			// horizontal spring
			// is particle isn't on the last collumn
			if (x < GRID_W - 1) {
				// create spring object
				Spring s;
				// connect particle at index with the particle to the right
				s.indexA = index;
				s.indexB = index + 1;
				// glm::distance calculates the distance between two vec3 positions
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = stiffness;
				// push back currrent springs to vector of Spring objects
				springs.push_back(s);
			}

			// vertical spring
			// is particle isn't on the very top row
			if (y < GRID_H - 1) {
				Spring s;
				// connect particle at index with the particle one row above it
				s.indexA = index;
				s.indexB = index + GRID_W;
				// glm::distance calculates the distance between two vec3 positions
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = stiffness;
				// push back currrent springs to vector of Spring objects
				springs.push_back(s);
			}

			// shear springs
			// diagonal springs (up-right)
			// if particle is not on the edge
			if (x < GRID_W-1 && y < GRID_H - 1) {
				// create spring
				Spring s;
				// connect a particle to the particle up one row and one particle right
				s.indexA = index;
				s.indexB = index + GRID_W + 1;
				// calculate natural rest length
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = sheerStiffness;
				springs.push_back(s);
			}

			// diagonal spring (up-left)
			if (x > 0 && x < GRID_W && y < GRID_H - 1) {
				// create spring
				Spring s;
				// connect a particle to the particle up one row and one particle right
				s.indexA = index;
				s.indexB = index + GRID_W - 1;
				// calculate natural rest length
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = sheerStiffness;
				springs.push_back(s);
			}

			// bend springs
			// bend spring horizonal
			if (x < GRID_W-2) {
				Spring s;
				s.indexA = index;
				s.indexB = index + 2;
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = bendStiffness;
				springs.push_back(s);
			}

			// bend spring vertical
			if (y < GRID_H - 2) {
				Spring s;
				s.indexA = index;
				s.indexB = index + (2 * GRID_W);
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = bendStiffness;
				springs.push_back(s);
			}
			
		}
	}

	// GL_LINES draws lines between pairs so we need a vector with a pair for every line to draw
	std::vector <unsigned int> indices;
	for (Spring& s : springs) {
		// take info from springs because each pring has a pair built into it
		indices.push_back(s.indexA);
		indices.push_back(s.indexB);
	}


	// buffer setup
	// holds ID for vertex array object
	unsigned int VAO;
	// holds ID for vertex buffer object
	unsigned int VBO;
	// holds ID for element buffer object
	unsigned int EBO;
	// generates ID for vertex array object and stores in VAO
	glGenVertexArrays(1, &VAO);
	// generates ID for vertex buffer object and stores in VBO
	glGenBuffers(1, &VBO);
	// generates ID for element buffer object
	glGenBuffers(1, &EBO);

	// makes our VAO currently active
	glBindVertexArray(VAO);
	// makes VBO the current active GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// makes EBO curreently active
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// allocates space for particles in the GPU
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(float) * 3, nullptr, GL_DYNAMIC_DRAW);
	// uploads indices vector to the gpu
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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
	// ground position
	float groundY = -0.9;

	// while the user has not closed the window 
	while (!glfwWindowShouldClose(window)) {
		//deltaTime tracking
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		applyForces(particles, deltaTime);
		// apply springs multiple times per loop to prevent extreme bouncing
		for(int i = 0; i < 5; i++) {
			applySprings(particles, springs);
		}
		
		// make sure the cloth doesn't go through the floor
		resolveGroundCollisions(particles, groundY);

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
		glClearColor(0.804f, 0.682f, 1.0f, 0.561f);
		// sets the color 
		glClear(GL_COLOR_BUFFER_BIT);
		// makes the draw call use the correct shader program
		glUseProgram(shaderProgram);
		// bind VAO before drawing
		glBindVertexArray(VAO);
		// draws the lines (shape to draw, how many lines, data type of indices, offest into the ebo)
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);


		// prevents screen from flickering by drawing to a back buffer and then swapping it to the front
		glfwSwapBuffers(window);
		// detects input events, without this line the program wouldnt respond to any input
		glfwPollEvents();
	}

	// cleanup
	glfwTerminate();
	return 0;
}