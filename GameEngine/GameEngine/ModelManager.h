#pragma once
/*#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>*/
#include <vector>
#include <string>
#include <memory>

#include "ResourceManager.h"
#include "BasicTypes.h"
#include "EffectedModel.h"

class ModelManager : public ResourceManager<EffectedModel>
{
public:
	static ModelManager* getInstance();

	bool loadModel(const std::string &modelName, std::shared_ptr<EffectedModel> model);

	// Doesn't initialize mesh, shader or material, assume all are loaded already.
	bool loadModel(const std::string &modelName, const std::string &meshName, const std::string &materialName);

	// Doesn't initialize mesh or shader, but initializes the material using the given shader
	// and texture.
	bool loadModel(const std::string &modelName, const std::string &meshName, const std::string &materialName,
		const std::string &shaderName, const std::string &diffuseTexture = "");

	// Initialize mesh, but not material or shader.
	bool loadModel(const std::string &modelName, const std::string &meshName, const std::string &materialName, PrimitiveType prim,
		const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);

	// Initialize material and shader, but not mesh.
	bool loadModel(const std::string &modelName, const std::string &meshName, const std::string& materialName,
		const std::string &shaderName, const std::string &vertexShaderPath, const std::string &fragmentShaderPath,
		const std::vector<ShaderVariable> &shaderVars, const std::string &diffuseTexture = "",
		bool printShaderLoadStatus = false);

	// Initialization of everything, mesh, material and shader.
	bool loadModel(const std::string &modelName, const std::string &meshName, const std::string &materialName,
		const std::string &shaderName, PrimitiveType prim, const std::vector<Vertex> &vertices,
		const std::vector<GLuint> &indices, const std::string &vertexShaderPath,
		const std::string &fragmentShaderPath, const std::vector<ShaderVariable> &shaderVars,
		const std::string &diffuseTexture = "", bool printShaderLoadStatus = false);
	
	// Easy load of cube model, where mesh, shader, material names aren't required to retrieve later.
	bool loadCubeModel(const std::string &modelName, const std::string &diffuseTexture);

	// Load cube, shader already loaded, just need material with texture
	bool loadCubeModel(const std::string &modelName, const std::string &meshName, const std::string &materialName,
		const std::string &shaderName, const std::string &diffuseTexture = "");

	// Load cube, first time shader loading.
	bool loadCubeModel(const std::string &modelName, const std::string &meshName, const std::string &materialName,
		const std::string &shaderName, const std::string &vertexShaderPath, const std::string &fragmentShaderPath,
		const std::vector<ShaderVariable> &shaderVars, const std::string &diffuseTexture = "",
		bool printShaderLoadStatus = false);

private:
	static std::unique_ptr<ModelManager> myInstance; // Singleton

	static const std::string defaultMeshName;
	static const std::string defaultMaterialName;
	static const std::string defaultShaderName;
	static const std::string defaultVertexShaderPath;
	static const std::string defaultFragmentShaderPath;
	static int materialIndex;

	ModelManager() {};
//	Mesh* buildSingleModel(const string &path, ImageManager &im, int _index = 0);
	//PolyMesh* buildPolyModel(const string &path, ImageManager &im);
};