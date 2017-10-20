#pragma once
#include "MeshType.h"
#include "SingleMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/*class PolyMesh : public MeshType
{
public:
	PolyMesh(vector<Mesh*> _meshes);
	~PolyMesh();

	vector<Mesh*>* getMeshes() { return &meshes; }
	int getNumMeshes() { return meshes.size(); }
	int getUniqueVertices(vector<Vec3> *verts); // Populates verts with all 
	the unique vertices.

	void setMeshes(vector<Mesh*> _meshes);

private:
	vector<Mesh*> meshes;
};*/

// Majority of this code was created using the help of 
// https://learnopengl.com/#!Model-Loading/Model

class PolyMesh : public MeshType
{
public:
	PolyMesh(std::string modelFilePath);
	std::vector<SingleMesh>* getMeshes();
	int getNumMeshes() const;

private:
	std::vector<SingleMesh> meshes;
	void loadNode(aiNode *node, const aiScene *scene);
	SingleMesh loadMesh(aiMesh *mesh, const aiScene *scene);

	const struct aiScene* scene = NULL;
};