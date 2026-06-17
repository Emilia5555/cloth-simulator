#pragma once
#include <vector>
#include "particle.h"
#include "spring.h"


// constant to represent gravity (-9.8 in y)
glm::vec3 GRAVITY = glm::vec3(0.0f, -9.8f, 0.0f);
float damping = 0.999f;

// changes position based on what forces are acting on the particle
// doesn't affect pinned particles
// particles: a vector of particle objects passed by reference
// deltaTime: a float representing time elapsed since last frame in seconds
void applyForces(std::vector<Particle>& particles, float deltaTime) {
	// loops through particles vector
	for (Particle& p : particles) {
		// skips pinned particles so they dont move
		if (!p.pinned) {
			// temporary to hold initial position
			glm::vec3 temp = p.position;
			// update position to position + (change in position) * acceleration * deltaTime^2
			// 0.98f * GRAVITY mimics acceleration due to gravity
			p.position += (p.position - p.previousPosition) * damping + (GRAVITY * deltaTime * deltaTime);
			p.previousPosition = temp;

		}
	}
}
// connects particels with springs
// corrects particle position to maintian natural rest lengths
// particles: a vector of particle objects passed by reference
// springs: a vector of spring objects passed by reference
void applySprings(std::vector<Particle>& particles, std::vector<Spring>& springs) {
	// make sure to pass vectors by reference so the actual values get changed
	// loops through springs vector
	for (Spring& s : springs) {
		// stores particle at current indexA
		Particle& a = particles[s.indexA];
		// stores particle at current indexB
		Particle& b = particles[s.indexB];

		// creates a vector representing the change in position
		glm::vec3 delta = b.position - a.position;
		// stores length of delta
		float currentLength = glm::length(delta);
		// prevent / 0 error
		if (currentLength == 0.0f) {
			continue;
		} 
		//create unit vector to get direction
		glm::vec3 direction = delta / currentLength;

		// finds how far the particle is from its natural length
		float displacement = currentLength - s.restLength;
		// determines how much to move the particle (what direction * how much)
		// 0.5f multiplier halves the correction since its being applied twice
		glm::vec3 correction = direction * displacement * 0.5f * s.stiffness;

		// if both are free move them each in equal and opposite directions
		if (!a.pinned && !b.pinned) {
			a.position += correction;
			b.position -= correction;
		}
		// if one a is pinned b gets the whole correction
		else if (a.pinned && !b.pinned) {
			b.position -= correction * 2.0f;
		}
		// if one b is pinned a gets the whole correction
		else if (b.pinned && !a.pinned) {
			a.position += correction * 2.0f;
		} 

	}
}

// prevents cloth from falling through the floor
// particles: vector of all particles
// groundY: y position of the gournd plane
void resolveGroundCollisions(std::vector<Particle>& particles, float groundY) {
	// checks y pos of every particle
	for (Particle& p : particles) {
		// if particle y is below y pos
		if (p.position.y < groundY) {
			// set all y pos to groundY
			p.position.y = groundY;
			p.previousPosition.y = groundY;
			// add friction to the ground
			// dampen horizontal velocity when it hits the ground
			p.previousPosition.x = p.position.x + (p.previousPosition.x - p.position.x) * 0.95f;
			p.previousPosition.z = p.position.z + (p.previousPosition.z - p.position.z) * 0.95f;
		}
	}
}