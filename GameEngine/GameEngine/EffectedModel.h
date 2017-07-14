#pragma once
#include <string>

#include "MeshManager.h"
#include "Renderable.h"

class Material;
class ShaderVariable;

// This is not inherited from SingleModel, so we can simply store the 
// names of the material and the model name. This prevents the model from
// loading being loaded multiple times.
class EffectedModel
{
public:
	EffectedModel() {};

	// Doesn't initialize mesh, shader or material, assume all are loaded already.
	EffectedModel(const std::string &meshName, const std::string &materialName);

	// Doesn't initialize mesh or shader, but initializes the material using the given shader
	// and texture.
	EffectedModel(const std::string &meshName, const std::string &materialName,
		const std::string &shaderName, const std::string &diffuseTexture = "");

	// Initialize mesh, but not material or shader.
	EffectedModel(const std::string &meshName, const std::string &materialName, PrimitiveType prim,
		const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);

	// Initialize material and shader, but not mesh.
	EffectedModel(const std::string &meshName, const std::string &materialName,
		const std::string &shaderName, const std::string &vertexShaderPath, const std::string &fragmentShaderPath,
		const std::vector<ShaderVariable> &shaderVars, const std::string &diffuseTexture = "",
		bool printShaderLoadStatus = false);

	// Initialization of everything, mesh, material and shader.
	EffectedModel(const std::string &meshName, const std::string &materialName,
		const std::string &shaderName, PrimitiveType prim, const std::vector<Vertex> &vertices,
		const std::vector<GLuint> &indices, const std::string &vertexShaderPath,
		const std::string &fragmentShaderPath, const std::vector<ShaderVariable> &shaderVars,
		const std::string &diffuseTexture = "", bool printShaderLoadStatus = false);

	void setMaterialName(const std::string &matName) { materialName = matName; }
	const std::string &getMaterialName() { return materialName; }
	Material* getMaterial();

	void setMeshName(const std::string &meshName) { this->meshName = meshName; }
	const std::string &getMeshName() { return meshName; }
	SingleMesh* getMesh(); // TODO: Change to MeshType when PolyMesh implemented.

	const std::string &name() const { return "EffectedModel"; }

	//BoundingBox* getBoundingBox() { return boundingBox; }

protected:
	void initMaterial(const std::string &materialName, const std::string &shaderName, const std::string &vertexShaderPath,
		const std::string &fragmentShaderPath, const std::vector<ShaderVariable> &shaderVars,
		const std::string &texturePath = "", bool printShaderLoadStatus = false);
	void initMaterialWithLoadedShader(const std::string &materialName, const std::string &shaderName, const std::string &texturePath);
	void initModel(PrimitiveType prim, const std::string &modelName, const std::vector<Vertex> &vertices,
		const std::vector<GLuint> &indices);

	void bindAttributes();

	std::string meshName;
	std::string materialName;
};