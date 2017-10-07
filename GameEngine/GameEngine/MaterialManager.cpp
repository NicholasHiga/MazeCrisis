#include "MaterialManager.h"
#include "TextureManager.h"
#include "ShaderVariable.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

unique_ptr<MaterialManager> MaterialManager::myInstance = nullptr;

MaterialManager*
MaterialManager::getInstance()
{
	if (myInstance == nullptr)
		myInstance = unique_ptr<MaterialManager>(new MaterialManager());

	return myInstance.get();
}

bool
MaterialManager::loadMaterial(const string &materialName, const string &shaderName, const string &diffuseTexture)
{
	shared_ptr<Material> mat = std::make_shared<Material>(materialName, shaderName);

	if (diffuseTexture != "")
	{
		if (!TextureManager::getInstance()->get(diffuseTexture))
			TextureManager::getInstance()->loadTexture(diffuseTexture);
		mat->setDiffuseTexture(diffuseTexture);
	}

	return store(materialName, mat);
}

bool
MaterialManager::loadMaterial(const string &materialName, const string &shaderName, const string &vertexShaderPath,
	const string &fragmentShaderPath, const vector<ShaderVariable> &shaderVars,
	const string &diffuseTexturePath, bool printShaderLoadStatus)
{
	shared_ptr<Material> mat = std::make_shared<Material>(materialName, shaderName, 
		vertexShaderPath, fragmentShaderPath, shaderVars, printShaderLoadStatus);

	if (diffuseTexturePath != "")
	{
		TextureManager::getInstance()->loadTexture(diffuseTexturePath);
		mat->setDiffuseTexture(diffuseTexturePath);
	}

	return store(materialName, mat);
}