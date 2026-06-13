// lets me access OpenGL functions
#include <glad/glad.h>
// lets me create window and use GLFW functions
#include <GLFW/glfw3.h>
#include <iostream>
// incude shaders and shader functions
#include "shader_utils.h"
#include "shaders.h"

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Cloth Simulator", nullptr, nullptr);
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

	// test particle positions
	float vertices[] = {
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

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
	// uploads data into buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// (void*)0 means start reading at byte 0 of the buffer
	//tells open gl that VBO has vertex data where each vertex is three floats and the vertexes are three floats apart
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// enables input slot 0
	glEnableVertexAttribArray(0);


	// unbind VAO and VBO
	// binding 0 is unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// while the user has not closed the window 
	while (!glfwWindowShouldClose(window)) {
		// indicates color to set (red,green, blue, alpha) uses 0-1 scale, expects float values
		glClearColor(1.0f, 0.627f, 0.992f, 0.55f);
		// sets the color 
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glPointSize(10.0f);
		glDrawArrays(GL_POINTS, 0, 4);


		// prevents screen from flickering by drawing to a back buffer and then swapping it to the front
		glfwSwapBuffers(window);
		// detects input events, without this line the program wouldnt respond to any input
		glfwPollEvents();
	}

	// cleanup
	glfwTerminate();
	return 0;
}