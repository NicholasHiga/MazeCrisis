#include "EffectedModel.h"

#include "Material.h"
#include "ShaderProgram.h"
#include "ShaderVariable.h"
#include "MaterialManager.h"
#include "ModelManager.h"

using std::string;
using std::vector;

// Doesn't initialize mesh, shader or material, assume all are loaded already.
EffectedModel::EffectedModel(const string &meshName, const string &materialName)
{
	this->meshName = meshName;
	addExistingMaterial(materialName);
	bindAttributes();
}

// Doesn't initialize mesh or shader, but initializes the material using 
// the given shader and texture.
EffectedModel::EffectedModel(const string &meshName, const string &materialName,
	const string &shaderName, const string &diffuseTexture)
{
	this->meshName = meshName;
	initMaterialWithLoadedShader(materialName, shaderName, diffuseTexture);
}

// Initialize mesh, but not material or shader.
EffectedModel::EffectedModel(const string &meshName, const string &materialName, 
	PrimitiveType prim, const vector<Vertex> &vertices, 
	const vector<GLuint> &indices)
{
	addExistingMaterial(materialName);
	initModel(prim, meshName, vertices, indices);
}

// Initialize material and shader, but not mesh.
EffectedModel::EffectedModel(const string &meshName, const string &materialName, 
	const string &shaderName, const string &vertexShaderPath,
	const string &fragmentShaderPath, const vector<ShaderVariable> &shaderVars,
	const string &diffuseTexture,
	bool printShaderLoadStatus)
{
	this->meshName = meshName;
	initMaterial(materialName, shaderName, vertexShaderPath, fragmentShaderPath,
		shaderVars, diffuseTexture, printShaderLoadStatus);
}

// Initialization of everything, mesh, material and shader.
EffectedModel::EffectedModel(const string &meshName,
	const string &materialName, const string &shaderName, PrimitiveType prim,
	const vector<Vertex> &vertices, const vector<GLuint> &indices,	
	const string &vertexShaderPath, const string &fragmentShaderPath,
	const vector<ShaderVariable> &shaderVars, const string &diffuseTexture, 
	bool printShaderLoadStatus)
{
	initModel(prim, meshName, vertices, indices);
	initMaterial(materialName, shaderName, vertexShaderPath, fragmentShaderPath,
		shaderVars, diffuseTexture, printShaderLoadStatus);
}

void 
EffectedModel::initMaterial(const string &materialName,
	const string &shaderName, const string &vertexShaderPath,
	const string &fragmentShaderPath, const vector<ShaderVariable> &shaderVars,
	const string &diffuseTexture, bool printShaderLoadStatus)
{
	try 
	{
		MaterialManager::getInstance()->loadMaterial(materialName, shaderName, 
			vertexShaderPath, fragmentShaderPath, shaderVars, diffuseTexture,
			printShaderLoadStatus);
		addExistingMaterial(materialName);
		bindAttributes();
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

void 
EffectedModel::initMaterialWithLoadedShader(const string &materialName, 
	const string &shaderName, const string &diffuseTexture)
{
	try
	{
		MaterialManager::getInstance()->loadMaterial(materialName, shaderName, 
			diffuseTexture);
		addExistingMaterial(materialName);
		bindAttributes();
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

void 
EffectedModel::initModel(PrimitiveType prim, const string &modelName, 
	const vector<Vertex> &vertices,
	const vector<GLuint> &indices)
{
	try
	{
		MeshManager::getInstance()->loadMesh(modelName, prim, vertices,
			indices);
		setMeshName(modelName);
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

void
EffectedModel::addExistingMaterial(const std::string &materialName)
{
	materials.push_back(MaterialManager::getInstance()->get(materialName));
}

SingleMesh* 
EffectedModel::getMesh()
{
	return MeshManager::getInstance()->get(meshName);
}

void
EffectedModel::bindAttributes()
{
	glBindVertexArray(getMesh()->getVAO());

	// TO DO: MAKE SURE WORKING WITH MULTIPLE MATERIALS!!
	vector<ShaderVariable> vars = getMaterials()[0]->getShader()
		->getShaderVars();

	for (size_t i = 0; i < vars.size(); i++)
	{	
		glEnableVertexAttribArray(i);
		glBindBuffer(GL_ARRAY_BUFFER, getMesh()->getVBO());
		ENGINE_VAR eVar = vars[i].getEngineVarType();

		if (eVar == ENGINE_VAR::VERTEX_POSITION)
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		else if (eVar == ENGINE_VAR::VERTEX_NORMAL)
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		else if (eVar == ENGINE_VAR::VERTEX_UV)
			glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
		else if (eVar == ENGINE_VAR::VERTEX_COLOR)
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	}

	glBindVertexArray(0);
}