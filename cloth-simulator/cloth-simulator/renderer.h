#pragma once
#include <glad/glad.h>
#include <vector>
#include "particle.h"
#include "spring.h"

// creates and configures VAO, VBO, and EBO for rendering cloth mesh
// deletes anything in buffer objects before creating new ones
// VAO: int to hold vertex array object ID
// VBO: int to hold vertex buffer object ID
// EBO: int to hold element buffer object ID
// particles: vector of particles, used to allocate GPU memory to hold positions
// springs: vector of springs, used to create vector of pairs to be connected by springs
void setupBuffers(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, std::vector<Particle>& particles, std::vector<Spring>& springs)
{
	// buffer setup
	// clear anything that was in the buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// GL_LINES draws lines between pairs so we need a vector with a pair for every line to draw
	std::vector <unsigned int> indices;
	for (Spring& s : springs) {
		// take info from springs because each pring has a pair built into it
		indices.push_back(s.indexA);
		indices.push_back(s.indexB);
	}
	
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

}

// creates and configures VAO, VBO, and EBO for rendering sphere
// deletes anything in buffer objects before creating new ones
// sphereVAO: int to hold vertex array object ID
// sphereVBO: int to hold vertex buffer object ID
// sphereEBO: int to hold element buffer object ID
// sphereVertices: holds positions of sphere vertices stored as 3 floats
// sphereIndices: holds connections to be made between vertices in the sphere
void setupSphereBuffers(unsigned int& sphereVAO, unsigned int& sphereVBO, unsigned int& sphereEBO, std::vector<float>& sphereVertices, std::vector<unsigned int>& sphereIndices)
{
	// clear anything that was in the buffers
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteBuffers(1, &sphereVBO);
	glDeleteBuffers(1, &sphereEBO);
	// generate IDs numbers for buffers and store in variables
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);

	// bind buffers
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);

	// allocate space for sphere vertices and upload data
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);
	// allocate space for sphere indices and upload data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

	//tells open gl that VBO has vertex data where each vertex is three floats and the vertexes are three floats apart
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// enables input slot 0
	glEnableVertexAttribArray(0);

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}




// creates a vector of floats from particle positions and uploads that data to the GPU 
// called after every position update to sync CPU and GPU positions
// VBO: ID of vertex buffer object
// particles: vector of particles
void uploadPositions(unsigned int VBO, std::vector<Particle>& particles) 
{
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
}