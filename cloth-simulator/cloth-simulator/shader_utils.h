#pragma once
#include <glad/glad.h>
#include <iostream>

// compiles a GLSL shader
// prints error messege if compilation fails
// type: GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
// source: null-terminated string 
// returns: integer ID of compiled shader object on the GPU
unsigned int compileShader(unsigned int type, const char* source) {
	// create an empty shader object assigns its ID number to int shader
	unsigned int shader = glCreateShader(type);
	// gives the string from shader.h to the new shader object
	glShaderSource(shader, 1, &source, nullptr);
	// compiles the shader
	glCompileShader(shader);

	// acts like a boolean
	int success = 0;
	// my copy of the infolog, currently empty
	char infoLog[512];

	// checks if shader compiled and sets success depending on if it does
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	// if shader doesnt compile correctly
	if (!success) {
		// gets info log and puts it in my infoLog variable
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		// outputs error message
		std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
	}
	// return ID referencing shader object on GPU
	return shader;
}

// compiles and links vertex and fragment shaders into a shader program
// prints error message if linking fails
// deletes shader objects after linking
// vertexSource: null-terminated string containing vertex shader GLSL 
// fragmentSource: null-terminated string containing fragment shader GLSL 
unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource) {
	// compile vertex and fragment shaders
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
	
	// create a shader program object and store ID # as an int
	unsigned int program = glCreateProgram();
	// attach vertex and frangment shader to program object 
	glAttachShader(program,vertexShader);
	glAttachShader(program, fragmentShader);
	// link all attached shaders together 
	glLinkProgram(program);

	// variable to store result of linkProgram as a boolean 
	int success = 0;
	// variable to store infoLog
	char infoLog[512];
	// check if the link was succesfull and store result in success
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	// if success fails 
	if (!success) {
		//store infoLog in infoLog variable
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		// error message
		std::cerr << "shader linking failed:\n" << infoLog << std::endl;
	}
	// delete vertex and fragment shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;

}
