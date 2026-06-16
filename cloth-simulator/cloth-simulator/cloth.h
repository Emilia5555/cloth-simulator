#pragma once
// particle struct
#include "particle.h"
// physics and forces
#include "physics.h"
// spring
#include "spring.h"

void generateCloth(std::vector<Particle>& particles, std::vector<Spring>& springs, const int GRID_W, const int GRID_H,  float stiffness, float shearStiffness, float bendStiffness) 
{
	// make sure particles and springs arent holding anything
	particles.clear();
	springs.clear();
	// generate particles
	// loop through height and width
	for (int y = 0; y < GRID_H; y++) {
		for (int x = 0; x < GRID_W; x++) {
			// space points evenly across the grid
			// *2 -1 changes range of point from 0-1 to 0-2 to -1 to 1
			// make divisor a float to avoid integer division
			float px = (x / (float)(GRID_W - 1) * 1.8f - 0.9f);
			float py = (y / (float)(GRID_H - 1) * 1.3f - 0.4f);

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
			if (x == GRID_W - 1 && y == GRID_H - 1) {
				particles.back().pinned = true;
			}

		}
	}

	// generate springs
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
			if (x < GRID_W - 1 && y < GRID_H - 1) {
				// create spring
				Spring s;
				// connect a particle to the particle up one row and one particle right
				s.indexA = index;
				s.indexB = index + GRID_W + 1;
				// calculate natural rest length
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = shearStiffness;
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
				s.stiffness = shearStiffness;
				springs.push_back(s);
			}

			// bend springs
			// bend spring horizonal
			if (x < GRID_W - 2) {
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
}