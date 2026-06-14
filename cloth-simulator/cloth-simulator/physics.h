#pragma once
#include <vector>
#include "particle.h"

// constant to represent gravity (-9.8 in y)
const glm::vec3 GRAVITY = glm::vec3(0.0f, -9.8f, 0.0f);

// applies forces to particles, takes in a particles vector and a float indicating time
void applyForces(std::vector<Particle>& particles, float deltaTime) {
	// loops through particles vector
	for (Particle& p : particles) {
		// skips pinned particles so they dont move
		if (!p.pinned) {
		// adds gravitational acceleration
		p.velocity += GRAVITY * deltaTime;
		// moves each particle by its current velocity
		p.position += p.velocity * deltaTime;
		}
	}
}