# cloth-simulator

A real-time cloth simulation built in C++ using OpenGL, GLM, and Dear ImGui

## Overview

This project implements a mass-spring cloth simulation using Verlet integration. 
A 50x50 grid of particles is connected by structural, shear, and bend springs that 
model the physical behavior of cloth. The simulation runs in real time with an 
interactive control panel for tuning physics parameters.

## Features

- Mass-spring particle system with 1600 particles
- Structural, shear, and bend spring constraints
- Verlet integration for stable physics simulation
- Position-based spring correction with multiple solver iterations
- Ground plane collision with friction
- Real-time parameter controls via Dear ImGui
- OpenGL 3.3 Core Profile rendering with VAO/VBO/EBO

## Controls

Use the Cloth Controls panel to adjust:
- **Gravity** — strength and direction of gravitational force
- **Damping** — how quickly the cloth loses energy and settles
- **Stiffness** — structural spring resistance to stretching
- **Shear Stiffness** — resistance to diagonal deformation
- **Bend Stiffness** — resistance to sharp folding
- **Ground Y** — height of the ground collision plane
- **Reset** — regenerates the cloth with original parameters

## Built With

- C++
- OpenGL 3.3
- GLFW — windowing and input
- GLAD — OpenGL function loading
- GLM — mathematics library
- Dear ImGui — real-time parameter controls
- vcpkg — package management

## Build Instructions

Requires Visual Studio 2022 with the Desktop Development with C++ workload and vcpkg integrated.

1. Clone the repository
2. Open `cloth-simulator.sln` in Visual Studio
3. Build and run with Ctrl+F5

## Demo

**Hanging cloth simulation**
[Watch on YouTube](https://www.youtube.com/watch?v=v2ppRnemSzA)

**Cloth draped over sphere with collision**
[Watch on YouTube](https://www.youtube.com/watch?v=8psz9TMvUZg)
