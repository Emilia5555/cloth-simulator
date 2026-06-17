#pragma once
// particle struct
#include "particle.h"
// physics and forces
#include "physics.h"
// spring struct
#include "spring.h"

// Generates a grid of particles, connected by springs, that represent the mesh of a cloth
// Clears particles and springs
// particles: vector of particle objects
// springs: a vector of spring objetcs
// gridW: number of particles along the width of the grid (int)
// gridH: number of particles along the hieght of the grid (int)
// stiffness: float representing how resistant the structural springs are to stretching
// shearStiffness: float representing how resistant the shear springs are to stretching
// bendStiffness: float representing how resistant the bend springs are to stretching
void generateCloth(std::vector<Particle>& particles, std::vector<Spring>& springs, const int gridW, const int gridH,  float stiffness, float shearStiffness, float bendStiffness) 
{
	// make sure particles and springs arent holding anything
	particles.clear();
	springs.clear();
	// generate particles
	// loop through height and width
	for (int y = 0; y < gridH; y++) {
		for (int x = 0; x < gridW; x++) {
			// space points evenly across the grid
			// *2 -1 changes range of point from 0-1 to 0-2 to -1 to 1
			// used smaller values than 2 and 1 to make sure cloth is always in frame
			// make divisor a float to avoid integer division
			float px = (x / (float)(gridW - 1) * 1.8f - 0.9f);
			float py = (y / (float)(gridH - 1) * 1.3f - 0.4f);

			// make a particle object 
			Particle p;
			//set position to the variables we calculated earlier
			p.position = glm::vec3(px, py, 0.0f);
			// set velocity to 0
			p.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			// particles aren't pinned by defualt
			p.pinned = false;
			p.previousPosition = p.position;
			// add to vector of Particles
			particles.push_back(p);

			// pin top left corrner
			if (x == 0 && y == gridH - 1) {
				particles.back().pinned = true;
			}
			//pin top right corrner
			if (x == gridW - 1 && y == gridH - 1) {
				particles.back().pinned = true;
			}

		}
	}

	// generate springs
	for (int y = 0; y < gridH; y++) {
		for (int x = 0; x < gridH; x++) {
			// converts 2D grid coordinates into a 1D vector
			int index = y * gridW + x;

			// horizontal springs
			// if particle isn't on the last collumn
			if (x < gridW - 1) {
				// create spring object
				Spring s;
				// connect particle at index with the particle to the right
				s.indexA = index;
				s.indexB = index + 1;
				// glm::distance calculates the distance between two vec3 positions
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				// set stiffness
				s.stiffness = stiffness;
				// push back currrent springs to vector of Spring objects
				springs.push_back(s);
			}

			// vertical springs
			// if particle isn't on the very top row
			if (y < gridH - 1) {
				Spring s;
				// connect particle at index with the particle one row above it
				s.indexA = index;
				s.indexB = index + gridW;
				// glm::distance calculates the distance between two vec3 positions
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = stiffness;
				// push back currrent springs to vector of Spring objects
				springs.push_back(s);
			}

			// shear springs
			// diagonal springs (up-right)
			// if particle is not on the edge
			if (x < gridW - 1 && y < gridH - 1) {
				// create spring
				Spring s;
				// connect a particle to the particle up one row and one particle right
				s.indexA = index;
				s.indexB = index + gridW + 1;
				// calculate natural rest length
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = shearStiffness;
				springs.push_back(s);
			}

			// diagonal spring (up-left)
			if (x > 0 && x < gridW && y < gridH - 1) {
				// create spring
				Spring s;
				// connect a particle to the particle up one row and one particle right
				s.indexA = index;
				s.indexB = index + gridW - 1;
				// calculate natural rest length
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = shearStiffness;
				springs.push_back(s);
			}

			// bend springs
			// bend spring horizonal
			if (x < gridW - 2) {
				Spring s;
				s.indexA = index;
				s.indexB = index + 2;
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = bendStiffness;
				springs.push_back(s);
			}

			// bend spring vertical
			if (y < gridH - 2) {
				Spring s;
				s.indexA = index;
				s.indexB = index + (2 * gridW);
				s.restLength = glm::distance(particles[s.indexA].position, particles[s.indexB].position);
				s.stiffness = bendStiffness;
				springs.push_back(s);
			}

		}
	}
}