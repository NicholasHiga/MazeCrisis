#include "ModelManager.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

unique_ptr<ModelManager> ModelManager::myInstance = nullptr;

ModelManager*
ModelManager::getInstance()
{
	if (myInstance == nullptr)
		myInstance = unique_ptr<ModelManager>(new ModelManager());

	return myInstance.get();
}

bool 
ModelManager::loadModel(const string &modelName, const string &filepath)
{
	return false;
}

bool 
ModelManager::loadModel(const string &modelName, 
	shared_ptr<EffectedModel> model)
{
	return store(modelName, std::move(model));
}

/*MeshType*
ModelManager::loadModel(const string &path, string &name,
	ImageManager &im, int _index)
{
	// First use asset importer and linitially load the model using the
	// following parameters.
	Assimp::Importer importer;
	scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	MeshType *mesh;
	// Polymodel = multiple meshes in single model, single model = single mesh
	// in single model.
	/*if (scene->mNumMeshes == 1)
		mesh = buildSingleModel(path, im, _index);
	else
		mesh = buildPolyModel(path, im);*/
	//store(name, mesh);
	//return mesh;
	//return nullptr;
//}
/*
Mesh*
ModelManager::buildSingleModel(const string &path, ImageManager &im, int _index)
{
	const aiMesh* mesh = scene->mMeshes[_index];
	aiVector3D* vertices = mesh->mVertices;
	Face *faces = new Face[mesh->mNumFaces];

	// Load each face.
	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		Vertex *verts = new Vertex[mesh->mFaces[i].mNumIndices];

		// Load vertices for the face.
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
		{
			unsigned int index = mesh->mFaces[i].mIndices[j];
			verts[j] = Vertex(Vec3(mesh->mVertices[index].x,
			mesh->mVertices[index].y, mesh->mVertices[index].z));

			// Vertex has color defined in the file.
			if (mesh->HasVertexColors(0))
			{
				aiColor4D color = mesh->mColors[0][index];
				verts[j].SetColor(Color(color.r, color.g, color.b, color.a));
			}

			// Vertex normals.
			if (mesh->HasNormals())
			{
				aiVector3D normal = mesh->mNormals[index];
				verts[j].Normal(Vec3(normal.x, normal.y, normal.z));
			}

			// UV/UVW coordinates.
			if (mesh->HasTextureCoords(0))
			{
				aiVector3D uvw = mesh->mTextureCoords[0][index];
				if (mesh->GetNumUVChannels() == 2)
					verts[j].setUV(uvw.x, uvw.y);
				else
					verts[j].setUVW(uvw.x, uvw.y, uvw.z);
			}
		}

		// Texturing.
		if (scene->HasMaterials())
		{
			unsigned int matIndex = mesh->mMaterialIndex;
			aiMaterial* aiMat = scene->mMaterials[matIndex];

			aiString texturePath;

			if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath)
				== AI_SUCCESS)
				GameEngine::loadImage(texturePath.C_Str(), 
				Helper::getFullPath(path, texturePath.C_Str()));

			faces[i] = Face(Helper::numVertsToPrimType(
				mesh->mFaces[i].mNumIndices), verts, texturePath.C_Str());
		}
		else
			faces[i] = Face(Helper::numVertsToPrimType(
				mesh->mFaces[i].mNumIndices), verts);
	}
	return new Mesh(faces, mesh->mNumFaces);
}

PolyMesh*
ModelManager::buildPolyModel(const string &path, ImageManager &im)
{
	// Load each mesh using buildSingleModel method.
	vector<Mesh*> meshes;
	for (int i = 0; i < scene->mNumMeshes; ++i)
		meshes.push_back(buildSingleModel(path, im, i));
	return new PolyMesh(meshes);
}*/