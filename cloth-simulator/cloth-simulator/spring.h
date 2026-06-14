#pragma once
#include "particle.h"

struct Spring {
	// index of first particle
	int indexA;
	// index of second particle
	int indexB;
	// natural distance between the two particles
	float restLength;
	// stiffness constant
	float stiffness;
};

