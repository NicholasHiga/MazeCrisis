#pragma once
#include <vector>
#include <string>
#include <memory>

#include "SingleMesh.h"
#include "ResourceManager.h"

class MeshManager : public ResourceManager<SingleMesh>
{
public:
	static MeshManager* getInstance();

	bool loadMesh(const std::string &meshName, std::shared_ptr<SingleMesh> model);
	bool loadMesh(const std::string &meshName, PrimitiveType prim, const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);
	bool loadCubeMesh(const std::string &meshName);
	bool loadSkyboxMesh(const std::string &meshName);

private:
	static std::unique_ptr<MeshManager> myInstance; // Singleton

	MeshManager();
	//	Mesh* buildSingleModel(const string &path, ImageManager &im, int _index = 0);
	//PolyMesh* buildPolyModel(const string &path, ImageManager &im);
	
	// Predone cube
	const std::vector<GLfloat> CUBE_POSITIONS =
	{
		-0.5, -0.5,  0.5,
		0.5, -0.5,  0.5,
		0.5,  0.5,  0.5,
		-0.5,  0.5,  0.5,
		// top
		-0.5,  0.5,  0.5,
		0.5,  0.5,  0.5,
		0.5,  0.5, -0.5,
		-0.5,  0.5, -0.5,
		// back
		0.5, -0.5, -0.5,
		-0.5, -0.5, -0.5,
		-0.5,  0.5, -0.5,
		0.5,  0.5, -0.5,
		// bottom
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, -0.5,  0.5,
		-0.5, -0.5,  0.5,
		// left
		-0.5, -0.5, -0.5,
		-0.5, -0.5,  0.5,
		-0.5,  0.5,  0.5,
		-0.5,  0.5, -0.5,
		// right
		0.5, -0.5,  0.5,
		0.5, -0.5, -0.5,
		0.5,  0.5, -0.5,
		0.5,  0.5,  0.5,
	};

	std::vector<GLfloat> SKYBOX_VERTS;

	const std::vector<GLfloat> CUBE_COLORS =
	{
		// front colors
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 0.0,
		// back colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		// front colors
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 0.0,
		// back colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		// front colors
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 0.0,
		// back colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
	};

	const std::vector<GLfloat> CUBE_UVS =
	{
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0
	};

	const std::vector<GLuint> CUBE_INDICES =
	{
		// front
		0,  1,  2,
		2,  3,  0,
		// top
		4,  5,  6,
		6,  7,  4,
		// back
		8,  9, 10,
		10, 11,  8,
		// bottom
		12, 13, 14,
		14, 15, 12,
		// left
		16, 17, 18,
		18, 19, 16,
		// right
		20, 21, 22,
		22, 23, 20
	};
};