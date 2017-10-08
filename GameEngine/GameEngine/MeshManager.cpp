#include "MeshManager.h"
#include <glm/glm.hpp>

using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

unique_ptr<MeshManager> MeshManager::myInstance = nullptr;

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

bool
MeshManager::loadMesh(const string &meshName, shared_ptr<SingleMesh> model)
{
	return store(meshName, model);
}

bool
MeshManager::loadMesh(const string &meshName, PrimitiveType prim, const vector<Vertex> &vertices, const vector<GLuint> &indices)
{
	return store(meshName, std::make_shared<SingleMesh>(prim, vertices, indices));
}

bool 
MeshManager::loadCubeMesh(const string &meshName)
{
	vector<Vertex> cubeVertices;
	for (size_t i = 0; i < CUBE_POSITIONS.size() / 3; ++i)
	{
		Vertex v;
		v.position = glm::vec3(CUBE_POSITIONS[i * 3], CUBE_POSITIONS[i * 3 + 1], CUBE_POSITIONS[i * 3 + 2]);
		v.uv = glm::vec2(CUBE_UVS[i * 2], CUBE_UVS[i * 2 + 1]);
		v.color = glm::vec3(CUBE_COLORS[i * 3], CUBE_COLORS[i * 3 + 1], CUBE_COLORS[i * 3 + 2]);
		cubeVertices.push_back(v);
	}

	return store(meshName, std::make_shared<SingleMesh>
		(PrimitiveType::PRIM_TRIANGLE, cubeVertices, CUBE_INDICES));
}

bool 
MeshManager::loadSkyboxMesh(const std::string &meshName)
{
	vector<Vertex> skyboxVerts;
	vector<GLuint> empty;
	for (size_t i = 0; i < SKYBOX_VERTS.size() / 3; ++i)
	{
		Vertex v;
		v.position = glm::vec3(SKYBOX_VERTS[i * 3], SKYBOX_VERTS[i * 3 + 1], SKYBOX_VERTS[i * 3 + 2]);
		skyboxVerts.push_back(v);
	}

	return store(meshName, std::make_shared<SingleMesh>
		(PrimitiveType::PRIM_TRIANGLE, skyboxVerts, empty));
}