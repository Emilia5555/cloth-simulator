#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Generates positions for dots along sphere and indices for wiresframe
// center: center of the sphere
// radius: radius of the sphere
// latSteps: how many lattitude divisons from top to bottom
// longSteps: how many longitude divisions from +x to -x
// outVertices: outputs a vector of floats representing x, y, z positions
// outIndices: outputs a vector of ints that represent the connections the grid lines make 
void generateSphere(glm::vec3 center, float radius, int latSteps, int longSteps, 
	std::vector<float>& outVertices, std::vector<unsigned int>& outIndices) 
{
	// loop through latitude steps to get angles from 0 to pi
	for (int i = 0; i <= latSteps; i++) 
	{
		// lattitude is the angle i (pi) / latSteps (0 is north pole, pi is south pole)
		float latitude = ((float)i / latSteps) *  glm::pi<float>();

		// loop through longitude steps to get angles from 0 to 2pi
		for (int j = 0; j <= longSteps; j++) 
		{
			// longitude is the angle j (pi) / longSteps (0 is +x, pi/2 is -z, pi is -x)
			float longitude = (float)j / longSteps * 2 *glm::pi<float>();

			// spherical coordinates to cartesian
			// add the center of the circle to start at the correct point
			// x = rcos(long) where r = r sin(lat)
			float x = center.x + radius * sin(latitude) * cos(longitude);
			// y = rcos(lat) because we measure from y-axis
			float y = center.y + radius * cos(latitude);
			// x = rsin(long) where r = r sin(lat)
			float z = center.z + radius * sin(latitude) * sin(longitude);

			// add those positions to outVertices
			outVertices.push_back(x);
			outVertices.push_back(y);
			outVertices.push_back(z);
		}
	}

	// build line indices
	// push back two neighbors at a time, vertical and horizontal
	// loop through latSteps like height
	for (int i = 0; i < latSteps; i++) 
	{
		// loop through longSteps like width
		for (int j = 0; j < longSteps; j++)
		{
			// current point runs from 0 to (lat + 1)(long + 1) - 1
			int current = i * (longSteps + 1) + j;
			// horizontal neighbor
			int next = current + 1;
			// vertical neighbor
			int below = current + (longSteps + 1);

			// horizontal connections
			outIndices.push_back(current);
			outIndices.push_back(next);

			// vertical connections
			outIndices.push_back(current);
			outIndices.push_back(below);
		}
	}


}