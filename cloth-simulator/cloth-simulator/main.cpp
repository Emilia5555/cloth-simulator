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
#include "cloth.h"

//buffer setup
#include "renderer.h"
// imgui controls
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// matrix
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//camera
#include "camera.h"
// sphere
#include "sphere.h"


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

	// consts for window size
	const float WINDOW_HEIGHT = 1200;
	const float WINDOW_WIDTH = 1000;

	// creates the window (width px, height px, title, monitor for fullscreen, share context)
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cloth Simulator", nullptr, nullptr);
	// if window doesn't get created, print error, terminate window, and exit
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		// avoid memory leak by terminating GLFW before exiting
		glfwTerminate();
		return -1;
	}

	// adjusts where the window pops up on screen (px from top and left of your screen)
	glfwSetWindowPos(window, 1500, 100);


	//applies the OpenGL context to the window, makes OpenGL work with the window we just made
	glfwMakeContextCurrent(window);

	//initialize GLAD to make GLFW work with windows
	//use glfwGetProcAdress from GLFW to show GLAD where to get the functions we need 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialze GLAD" << std::endl;
		return -1;
	}

	// make the depth correct
	glEnable(GL_DEPTH_TEST);

	// ImGui setup
	// gotta have this
	IMGUI_CHECKVERSION();
	// creates context ill be working with
	ImGui::CreateContext();
	// stores ImGui input/output configuration
	ImGuiIO& io = ImGui::GetIO();
	// connects ImGui to windows
	// false prevents ImGui callbacks from being replaced with mine
	ImGui_ImplGlfw_InitForOpenGL(window, false);
	// conects ImGui to OpenGl 3.3 renderer
	ImGui_ImplOpenGL3_Init("#version 330");
	// dark theme for control panel
	ImGui::StyleColorsDark();




	// create shader program object for vertex and fragment shader
	unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	// 10x 10 grid of particle
	const int GRID_W = 40;
	const int GRID_H = 40;



	// vector to hold particles
	std::vector<Particle> particles;
	// vector to hold springs
	std::vector <Spring> springs;
	// adjusts stiffness of springs
	const float DEFAULT_STIFFNESS = 1.2f; 
	const float DEFAULT_SHEAR = 1.0f;
	const float DEFAULT_BEND = 0.08f;

	float stiffness = DEFAULT_STIFFNESS;
	float shearStiffness = DEFAULT_SHEAR;
	float bendStiffness = DEFAULT_BEND;

	// call generate cloth
	generateCloth(particles, springs, GRID_W, GRID_H, stiffness, shearStiffness, bendStiffness);
	
	// buffer setup
	// holds ID for vertex array object
	unsigned int VAO = 0;
	// holds ID for vertex buffer object
	unsigned int VBO = 0;
	// holds ID for element buffer object
	unsigned int EBO = 0;
	


	setupBuffers(VAO, VBO, EBO, particles, springs);

	//time tracking
	float lastTime = glfwGetTime();
	// ground position
	float groundY = -0.9;

	// create camera
	Camera camera(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		45.0f, 
		(WINDOW_WIDTH/ WINDOW_HEIGHT),
		0.1f,
		100.0f
	);
	
	glfwSetWindowUserPointer(window,&camera);
	glfwSetMouseButtonCallback(window, mouseButtonCallBack);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetScrollCallback(window, scrollCallback);

	camera.updateOrbit();

	// setup sphere
	glm::vec3 sphereCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	float sphereRadius = 0.3f;
	std::vector<float> sphereVertices;
	std::vector<unsigned int> sphereIndices;
	generateSphere(sphereCenter, sphereRadius, 20, 20, sphereVertices, sphereIndices);

	// sphere buffers
	// variables to hold IDs for buffers
	unsigned int sphereVAO = 0;
	unsigned int sphereVBO = 0;
	unsigned int sphereEBO = 0;

	// generate IDs numbers for buffers and store in variables
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);

	// bind buffers
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);

	// allocate space for sphere vertices and upload data
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(),GL_STATIC_DRAW);
	// allocate space for sphere indices and upload data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);
	
	//tells open gl that VBO has vertex data where each vertex is three floats and the vertexes are three floats apart
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// enables input slot 0
	glEnableVertexAttribArray(0);

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// while the user has not closed the window 
	while (!glfwWindowShouldClose(window)) {
		// tells ImGui a new frame is starting
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// starts an ImGui window called cloth controls
		ImGui::Begin("Cloth Controls");
		// slider for gravity y component (range: -20 to 0)
		ImGui::SliderFloat("Gravity", &GRAVITY.y,-20.0f,0.0f);
		// slider for damping (range: 0.9 to 1.0)
		ImGui::SliderFloat("Damping", &damping, 0.9f, 1.0f);
		// sliders for stiffness (range: 0.0 to 10.0)
		ImGui::SliderFloat("Stiffness", &stiffness, 0.0f, 100.0f);
		ImGui::SliderFloat("Shear Stiffness", &shearStiffness, 0.0f, 10.0f);
		ImGui::SliderFloat("Bend Stiffness", &bendStiffness, 0.0f, 10.0f);
		// slider for y pos of center of sphere (-2 to 0)
		if (ImGui::SliderFloat("Sphere Center vertical", &sphereCenter.y, -2.0f, 2.0f)) 
		{
			generateSphere(sphereCenter, sphereRadius, 20, 20, sphereVertices, sphereIndices);

			// draw sphere
			glBindVertexArray(sphereVAO);
			glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);

			// allocate space for sphere vertices and upload data
			glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);
			// allocate space for sphere indices and upload data
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

		}
		// reset button
		// if reset button is pushed
		if (ImGui::Button("Reset")) 
		{
			// set all values to what they were initially
			stiffness = DEFAULT_STIFFNESS;
			shearStiffness = DEFAULT_SHEAR;
			bendStiffness = DEFAULT_BEND;
			GRAVITY = glm::vec3(0.0f, -9.8f, 0.0f);
			damping = 0.999f;
			groundY=-0.6f;

			//reset camera
			camera.reset();

			// regenerate the cloth
			generateCloth(particles, springs, GRID_W, GRID_H, stiffness, shearStiffness, bendStiffness);
			// call setupBuffers
			setupBuffers(VAO,VBO,EBO,particles,springs);
			

		}
		//end the ImGui window
		ImGui::End();

		//deltaTime tracking
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		// cap delta time to prevent it from breaking the simulation when the window is moved 
		deltaTime = glm::min(deltaTime,0.04f);
		lastTime = currentTime;

		applyForces(particles, deltaTime);
		// apply springs multiple times per loop to prevent weird behavior
		for(int i = 0; i < 40; i++) {
			applySprings(particles, springs);
			resolveSphereCollisions(particles, sphereCenter, sphereRadius);
		}

		// make sure the cloth doesn't go through the floor
		resolveGroundCollisions(particles, groundY);

		// upload position data
		uploadPositions(VBO, particles);

		// indicates color to set background (red,green, blue, alpha) uses 0-1 scale, expects float values
		glClearColor(0.067f, 0.071f, 0.102f, 1.0f);
		// sets the color 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// makes the draw call use the correct shader program
		glUseProgram(shaderProgram);

		// get view and projection matrix every frame
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = camera.getProjectionMatrix();

		// glGetUniformLocation() gets view uniform from shader and returns int ID for that variables slot 
		// glUniformMatrix4fv uploads a 4x4 float matrix to that slot on the GPU
		// requires the raw float data of the matrix
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		
		// bind VAO before drawing
		glBindVertexArray(VAO);
		// set color for mesh
		glUniform3f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f);
		// draws the lines for mesh (shape to draw, how many lines, data type of indices, offest into the ebo)
		glDrawElements(GL_LINES, springs.size() * 2, GL_UNSIGNED_INT, 0);

		// draw sphere
		glBindVertexArray(sphereVAO);
		// set color for sphere
		glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.4f, 0.4f, 0.451f);
		glDrawElements(GL_TRIANGLES, sphereIndices.size() , GL_UNSIGNED_INT, 0);


		// render ImGui controls
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// prevents screen from flickering by drawing to a back buffer and then swapping it to the front
		glfwSwapBuffers(window);
		// detects input events, without this line the program wouldnt respond to any input
		glfwPollEvents();
	}

	// cleanup
	glfwTerminate();
	return 0;
}