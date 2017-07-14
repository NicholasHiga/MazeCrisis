#include "Material.h"
#include <iostream>
#include "TextureManager.h"
#include "ShaderVariable.h"
#include "ShaderProgramManager.h"

Material::Material(std::string shaderName)
{
	setShader(shaderName);
}

Material::Material(std::string shaderName, std::string vertexShaderPath, std::string fragmentShaderPath,
	std::vector<ShaderVariable> shaderVars, bool printShaderLoadStatus)
{
	try
	{
		ShaderManagerLoadStatus status;
		status = ShaderProgramManager::getInstance()->loadShader(shaderName, vertexShaderPath,
			fragmentShaderPath, shaderVars);

		if (printShaderLoadStatus)
			std::cout << status.message << std::endl;

		if (status.shaderLoadStatus != OK)
			throw std::runtime_error(status.message);

		setShader(shaderName);
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

void 
Material::setShader(const std::string &shaderName)
{
	this->shaderName = shaderName;
}

void 
Material::setDiffuseTexture(const std::string &textureName)
{
	textures["diffuse"] = TextureManager::getInstance()->get(textureName);
}

GLuint 
Material::getShaderID()
{
	return ShaderProgramManager::getInstance()->get(shaderName)->getProgramID();
}

ShaderProgram*
Material::getShader()
{
	return ShaderProgramManager::getInstance()->get(shaderName);
}
