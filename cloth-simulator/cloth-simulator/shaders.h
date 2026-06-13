#pragma once

// vertex shader
const char* vertexShaderSource = R"(
	//indicates which version to use (3.3 core)
	#version 330 core
	// declares aPos as a 3D vector at location 0
	layout (location=0) in vec3 aPos;
	
	//main function that runs for every vertex
	void main() {
		// makes the vec3 aPos into a vec4 but just sets w to 1.0
		gl_Position = vec4(aPos,1.0);
	}
)";

// fragment shader
const char* fragmentShaderSource = R"(
	//indicates which version to use (3.3 core)
	#version 330 core
	// creates an output variable FragColor that is a vec4
	out vec4 FragColor;

	void main(){
		//sets the color of FragColor
		FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
)";