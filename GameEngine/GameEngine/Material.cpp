#include "Material.h"
#include <iostream>
#include "TextureManager.h"
#include "ShaderVariable.h"
#include "ShaderProgramManager.h"

Material::Material(const std::string &materialName, const std::string &shaderName)
{
	setMaterialName(materialName);
	setShader(shaderName);
}

Material::Material(const std::string &materialName, const std::string &shaderName, 
	const std::string &vertexShaderPath, const std::string &fragmentShaderPath,
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
		setMaterialName(materialName);
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

void 
Material::setMaterialName(const std::string &materialName)
{
	this->materialName = materialName;
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

const std::string
Material::getMaterialName()
{
	return materialName;
}
