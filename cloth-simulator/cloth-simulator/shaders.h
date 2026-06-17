#pragma once

// vertex shader
const char* vertexShaderSource = R"(
	//indicates which version to use (3.3 core)
	#version 330 core
	// declares aPos as a 3D vector at location 0
	layout (location=0) in vec3 aPos;
	
	// uniform means it doesnt change
	// accepts view and projection matrices as uniforms so the GPU can transform particle positions correctly
	uniform mat4 view;
	uniform mat4 projection;

	//main function that runs for every vertex
	void main() {
		// matricex mutiplication applies left to right
		// first view makes the world space into camera space
		// then projection adds perspective
		gl_Position = projection * view * vec4(aPos, 1);
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