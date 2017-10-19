#include "MeshManager.h"

#include <glm/glm.hpp>
#include "ShaderVariable.h"
#include "MaterialManager.h"
#include "ShaderProgramManager.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

unique_ptr<MeshManager> MeshManager::myInstance = nullptr;

// TODO: Add pathing such that the shaders are found regardless of the project
// location.
const std::string MeshManager::defaultMeshName = "DefaultCubeMesh.mesh";
const std::string MeshManager::defaultMaterialName = "DefaultCubeMaterial.mat";
const std::string MeshManager::defaultShaderName = "DefaultCubeShader.shader";
const std::string MeshManager::defaultVertexShaderPath
= "../GameEngine/DefaultShaders/DefaultVertex.vert";
const std::string MeshManager::defaultFragmentShaderPath
= "../GameEngine/DefaultShaders/DefaultFragment.frag";
int MeshManager::materialIndex = 0;

MeshManager*
MeshManager::getInstance()
{
	if (myInstance == nullptr)
		myInstance = std::unique_ptr<MeshManager>(new MeshManager());

	return myInstance.get();
}

MeshManager::MeshManager()
{
	SKYBOX_VERTS =
	{
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
}

// Initialize neither - assume both model, material are already loaded.
bool
MeshManager::loadModel(const string &modelName, const string &baseMeshName,
	const string &materialName)
{
	return store(modelName, std::make_shared<SingleMesh>(
		*MeshManager::getInstance()->get(baseMeshName),
		modelName, materialName));
}

// Doesn't initialize mesh or shader, but initializes the material using the
// given shader and texture.
bool
MeshManager::loadModel(const string &modelName, const string &baseMeshName,
	const string &materialName, const string &shaderName,
	const string &diffuseTexture)
{
	return store(modelName,
		std::make_shared<SingleMesh>(
			*MeshManager::getInstance()->get(baseMeshName),
			modelName, materialName, shaderName,
			diffuseTexture));
}

// Initialize mesh, but not material or shader.
bool
MeshManager::loadModel(const string &modelName, const string &meshName,
	const string &materialName, PrimitiveType prim,
	const vector<Vertex> &vertices, const vector<GLuint> &indices)
{
	return store(modelName,
		std::make_shared<SingleMesh>(meshName, materialName, prim, vertices,
			indices));
}

// Initialize material and shader, but not mesh.
bool
MeshManager::loadModel(const string &modelName, const string &baseMeshName,
	const string &materialName, const string &shaderName,
	const string &vertexShaderPath, const string &fragmentShaderPath,
	const vector<ShaderVariable> &shaderVars, const string &diffuseTexture,
	bool printShaderLoadStatus)
{
	return store(modelName,
		std::make_shared<SingleMesh>(
			*MeshManager::getInstance()->get(baseMeshName),
			modelName, materialName, shaderName,
			vertexShaderPath, fragmentShaderPath, shaderVars,
			diffuseTexture, printShaderLoadStatus));
}

// Initialization of everything.
bool
MeshManager::loadModel(const string &modelName, const string &meshName,
	const string &materialName, const string &shaderName, PrimitiveType prim,
	const vector<Vertex> &vertices, const vector<GLuint> &indices,
	const string &vertexShaderPath, const string &fragmentShaderPath,
	const vector<ShaderVariable> &shaderVars, const string &diffuseTexture,
	bool printShaderLoadStatus)
{
	return store(modelName,
		std::make_shared<SingleMesh>(meshName, materialName,
			shaderName, prim, vertices, indices, vertexShaderPath,
			fragmentShaderPath, shaderVars, diffuseTexture,
			printShaderLoadStatus));
}

bool
MeshManager::loadCubeModel(const std::string &modelName,
	const std::string &diffuseTexture)
{
	vector<ShaderVariable> defaultCubeShaderVars;
	defaultCubeShaderVars.push_back(ShaderVariable(
		ENGINE_VAR::VERTEX_POSITION, "position"));
	defaultCubeShaderVars.push_back(ShaderVariable(
		ENGINE_VAR::VERTEX_UV, "uv"));
	defaultCubeShaderVars.push_back(ShaderVariable(
		ENGINE_VAR::SAMPLER_2D, "textureIn"));
	defaultCubeShaderVars.push_back(ShaderVariable(
		ENGINE_VAR::MODEL_VIEW_MATRIX, "modelViewMatrix"));
	defaultCubeShaderVars.push_back(ShaderVariable(
		ENGINE_VAR::PROJECTION_MATRIX, "projectionMatrix"));

	if (!MeshManager::getInstance()->get(defaultMeshName))
		MeshManager::getInstance()->loadCubeMesh(defaultMeshName);

	std::string matName = defaultMaterialName + std::to_string(materialIndex);
	if (!MaterialManager::getInstance()->get(matName))
	{
		MaterialManager::getInstance()->loadMaterial(matName,
			defaultShaderName, diffuseTexture);
		MeshManager::materialIndex++;
	}

	if (!ShaderProgramManager::getInstance()->get(defaultShaderName))
		ShaderProgramManager::getInstance()->loadShader(defaultShaderName,
			defaultVertexShaderPath, defaultFragmentShaderPath,
			defaultCubeShaderVars);

	return loadModel(modelName, defaultMeshName, matName);
}

// Load cube, shader already loaded, just need material with texture
bool
MeshManager::loadCubeModel(const string &modelName, const string &meshName,
	const string &materialName, const string &shaderName,
	const string &diffuseTexture)
{
	if (!MeshManager::getInstance()->get(meshName))
		MeshManager::getInstance()->loadCubeMesh(meshName);

	return loadModel(modelName, meshName, materialName, shaderName,
		diffuseTexture);
}

// Load cube, first time shader loading.
bool
MeshManager::loadCubeModel(const string &modelName, const string &meshName,
	const string &materialName, const string &shaderName,
	const string &vertexShaderPath, const string &fragmentShaderPath,
	const vector<ShaderVariable> &shaderVars, const string &diffuseTexture,
	bool printShaderLoadStatus)
{
	if (!MeshManager::getInstance()->get(meshName))
		MeshManager::getInstance()->loadCubeMesh(meshName);

	return loadModel(modelName, meshName, materialName, shaderName,
		vertexShaderPath, fragmentShaderPath, shaderVars, diffuseTexture,
		printShaderLoadStatus);
}

bool
MeshManager::loadMesh(const string &meshName, shared_ptr<SingleMesh> model)
{
	return store(meshName, model);
}

bool
MeshManager::loadMesh(const string &meshName, PrimitiveType prim,
	const vector<Vertex> &vertices, const vector<GLuint> &indices)
{
	return store(meshName, std::make_shared<SingleMesh>(meshName, prim, 
		vertices, indices));
}

bool 
MeshManager::loadCubeMesh(const string &meshName)
{
	vector<Vertex> cubeVertices;
	for (size_t i = 0; i < CUBE_POSITIONS.size() / 3; ++i)
	{
		Vertex v;
		v.position = glm::vec3(CUBE_POSITIONS[i * 3], CUBE_POSITIONS[i * 3 + 1],
			CUBE_POSITIONS[i * 3 + 2]);
		v.uv = glm::vec2(CUBE_UVS[i * 2], CUBE_UVS[i * 2 + 1]);
		v.color = glm::vec3(CUBE_COLORS[i * 3], CUBE_COLORS[i * 3 + 1],
			CUBE_COLORS[i * 3 + 2]);
		cubeVertices.push_back(v);
	}

	return store(meshName, std::make_shared<SingleMesh>
		(meshName, PrimitiveType::PRIM_TRIANGLE, cubeVertices, CUBE_INDICES));
}

bool 
MeshManager::loadSkyboxMesh(const std::string &meshName)
{
	vector<Vertex> skyboxVerts;
	vector<GLuint> empty;
	for (size_t i = 0; i < SKYBOX_VERTS.size() / 3; ++i)
	{
		Vertex v;
		v.position = glm::vec3(SKYBOX_VERTS[i * 3], SKYBOX_VERTS[i * 3 + 1],
			SKYBOX_VERTS[i * 3 + 2]);
		skyboxVerts.push_back(v);
	}

	return store(meshName, std::make_shared<SingleMesh>
		(meshName, PrimitiveType::PRIM_TRIANGLE, skyboxVerts, empty));
}