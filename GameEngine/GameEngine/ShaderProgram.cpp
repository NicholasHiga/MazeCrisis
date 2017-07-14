#include "ShaderProgram.h"

#include <iostream>
#include <fstream>

#include "ShaderVariable.h"

using std::string;
using std::vector;

ShaderProgram::ShaderProgram(const string &vertexFilePath, const string &fragmentFilePath, const vector<ShaderVariable> &shaderVars)
{
	ShaderLoadStatus status = loadShaders(vertexFilePath, fragmentFilePath);
	if ((status.shaderLoadStatus & VERT_SHADER_NOT_FOUND) == VERT_SHADER_NOT_FOUND || 
		(status.shaderLoadStatus & FRAG_SHADER_NOT_FOUND) == FRAG_SHADER_NOT_FOUND || 
		(status.shaderLoadStatus & VERT_SHADER_COMPILATION_FAILED) == VERT_SHADER_COMPILATION_FAILED ||
		(status.shaderLoadStatus & FRAG_SHADER_COMPILATION_FAILED) == FRAG_SHADER_COMPILATION_FAILED ||
		(status.shaderLoadStatus & LINKING_FAILED) == LINKING_FAILED)
		throw std::runtime_error(status.message);

	setShaderVars(shaderVars);
}

ShaderLoadStatus
ShaderProgram::loadShaders(const string &vertexFilePath, const string &fragmentFilePath)
{
	ShaderLoadStatus status;
	status.shaderLoadStatus = OK;

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		// File not found.
		status.message = "Impossible to open " + vertexFilePath + ". Are you in the right directory?\n";
		status.shaderLoadStatus = VERT_SHADER_NOT_FOUND;
		return status;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragmentFilePath, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		status.message = "Impossible to open " + fragmentFilePath + ". Are you in the right directory?\n";
		status.shaderLoadStatus = FRAG_SHADER_NOT_FOUND;
		return status;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	//printf("Compiling shader : %s\n", vertexFilePath);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		//printf("%s\n", &VertexShaderErrorMessage[0]);
		status.message = string(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end());
		if (Result != GL_FALSE)
			status.shaderLoadStatus |= VERT_SHADER_WARNING;
		else
		{
			status.shaderLoadStatus |= VERT_SHADER_COMPILATION_FAILED;
			return status;
		}
	}


	// Compile Fragment Shader
	//printf("Compiling shader : %s\n", fragmentFilePath);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		//printf("%s\n", &FragmentShaderErrorMessage[0]);
		status.message +=  " " + string(FragmentShaderErrorMessage.begin(), FragmentShaderErrorMessage.end());
		if (Result != GL_FALSE)
			status.shaderLoadStatus |= FRAG_SHADER_WARNING;
		else
		{
			status.shaderLoadStatus |= FRAG_SHADER_COMPILATION_FAILED;
			return status;
		}
	}


	// Link the program
	//printf("Linking program\n");
	programID = glCreateProgram();
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		//printf("%s\n", &ProgramErrorMessage[0]);
		status.message += " " + string(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
		if (Result != GL_FALSE)
			status.shaderLoadStatus |= LINKING_WARNINGS;
		else
		{
			status.shaderLoadStatus |= LINKING_FAILED;
			return status;
		}
	}


	glDetachShader(programID, VertexShaderID);
	glDetachShader(programID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	this->vertexFilePath = vertexFilePath;
	this->fragmentFilePath = fragmentFilePath;

	return status;
}

void
ShaderProgram::setShaderVars(vector<ShaderVariable> shaderVars)
{
	this->shaderVars = shaderVars; 
	for (size_t i = 0; i < shaderVars.size(); i++) 
	{
		if (shaderVars[i].getShaderVarType() == SHADER_VAR_TYPE::UNIFORM)
		{
			shaderVars[i].setUniformLocation(glGetUniformLocation(programID, 
				shaderVars[i].getShaderVarName().c_str()));
			if (shaderVars[i].getEngineVarType() == ENGINE_VAR::MODEL_VIEW_MATRIX)
				modelViewMatrixID = shaderVars[i].getUniformLocation();
			else if (shaderVars[i].getEngineVarType() == ENGINE_VAR::PROJECTION_MATRIX)
				projectionID = shaderVars[i].getUniformLocation();		

			if (shaderVars[i].getEngineVarType() == ENGINE_VAR::SAMPLER_2D
				|| shaderVars[i].getEngineVarType() == ENGINE_VAR::SAMPLER_CUBE)
				samplerVars.push_back(shaderVars[i]);
		}
	}
}

void 
ShaderProgram::pushShaderVar(ShaderVariable shaderVar)
{ 
	shaderVars.push_back(shaderVar);

	if (shaderVar.getEngineVarType() == ENGINE_VAR::SAMPLER_2D)
		samplerVars.push_back(shaderVar);
};