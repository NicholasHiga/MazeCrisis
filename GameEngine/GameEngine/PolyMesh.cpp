#include "PolyMesh.h"

/*PolyMesh::PolyMesh(vector<Mesh*> _meshes)
{
	boundingBox = nullptr;
	setMeshes(_meshes);
}

PolyMesh::~PolyMesh()
{
	if (boundingBox)
		delete boundingBox;
}

int 
PolyMesh::getUniqueVertices(vector<Vec3> *verts)
{
	if (meshes.size() >= 1)
	{
		meshes[0]->getUniqueVertices(verts);
		for (int i = 1; i < meshes.size(); ++i)
		{
			bool isVertInExisting = false;
			vector<Vec3> temp;
			meshes[i]->getUniqueVertices(&temp);

			for (int j = 0; j < temp.size(); ++j)
			{
				for (int k = 0; k < verts->size(); ++k)
				{
					if ((*verts)[k] == (temp)[j])
					{
						isVertInExisting = true;
						break;
					}
				}

				if (!isVertInExisting)
					verts->push_back((temp)[j]);
			}
		}

		return verts->size();
	}
}

void
PolyMesh::setMeshes(vector<Mesh*> _meshes)
{
	meshes = _meshes;

	if (boundingBox)
		delete boundingBox;

	boundingBox = new BoundingBox(*this);
}
*/

using std::vector;
/*
PolyMesh::PolyMesh(std::string modelFilePath)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(modelFilePath, aiProcess_FlipUVs |
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
		| aiProcess_SortByPType);

	// TODO: Look up more flags to use 
	// http://assimp.sourceforge.net/lib_html/
	// postprocess_8h.html#a64795260b95f5a4b3f3dc1be4f52e410

	// aiProcess_GenNormals, aiProcess_RemoveRedundantMaterials, 
	// aiProcess_FindInvalidData, aiProcess_OptimizeMeshes, 
	// aiProcess_OptimizeGraph, aiProcess_SplitLargeMeshes 
	// seem to be of interest.

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
		|| !scene->mRootNode)
	{
		throw std::runtime_error("Scene not loaded from assimp.");
		return;
	}
	loadNode(scene->mRootNode, scene);
}

void 
PolyMesh::loadNode(aiNode *node, const aiScene *scene)
{
	for (size_t i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(loadMesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; ++i)
		loadNode(node->mChildren[i], scene);
}

SingleMesh 
PolyMesh::loadMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;

		if (mesh->HasPositions())
			vertex.position = glm::vec3(mesh->mVertices[i].x,
				mesh->mVertices[i].y, mesh->mVertices[i].z);

		// TODO: Double check that uvw coords are compatible with
		// only uv coords.
		if (mesh->HasTextureCoords(0))
			vertex.uv = glm::vec2(mesh->mTextureCoords[i]->x, 
				mesh->mTextureCoords[i]->y);

		if (mesh->HasNormals())
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
				mesh->mNormals[i].z);

		if (mesh->HasVertexColors(0))
			vertex.color = glm::vec3(mesh->mColors[i]->r, mesh->mColors[i]->g,
				mesh->mColors[i]->b);

		vertices.push_back(vertex);
	}

	// process indices
	for (size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}
	
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		/// TODO: Figure out where to input textures
	}

	return SingleMesh(" ", PrimitiveType::PRIM_TRIANGLE, vertices, indices);
}

std::vector<SingleMesh>* 
PolyMesh::getMeshes()
{
	return &meshes;
}

int 
PolyMesh::getNumMeshes() const
{
	return meshes.size();
}*/