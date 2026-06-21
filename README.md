# Cloth Simulator

A real-time cloth simulation built from scratch in C++ using OpenGL, GLM, and Dear ImGui.

## Overview

This project implements a mass-spring cloth simulation using Verlet integration. A 40x40 grid of particles is connected by structural, shear, and bend springs that model the physical behavior of cloth. The simulation runs in real time with an interactive control panel for tuning physics parameters, a 3D orbit camera, and sphere collision.

## Features

- Mass-spring particle system with 1600 particles
- Structural, shear, and bend spring constraints
- Verlet integration for stable physics simulation
- Position-based spring correction with multiple solver iterations
- Ground plane collision with friction
- Sphere collision with surface friction
- 3D perspective camera with mouse orbit and scroll zoom
- Real-time parameter controls via Dear ImGui
- OpenGL 3.3 Core Profile rendering with VAO/VBO/EBO

## Controls

**Camera**
- Left click and drag — orbit around the scene
- Scroll wheel — zoom in and out

**Cloth Controls panel**
- **Gravity** — strength of gravitational force
- **Damping** — how quickly the cloth loses energy and settles
- **Stiffness** — structural spring resistance to stretching
- **Shear Stiffness** — resistance to diagonal deformation
- **Bend Stiffness** — resistance to sharp folding
- **Sphere Y** — vertical position of the collision sphere
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
