#include "ModelManager.h"
#include "ShaderVariable.h"
#include "MaterialManager.h"
#include "ShaderProgramManager.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

unique_ptr<ModelManager> ModelManager::myInstance = nullptr;

// TODO: Add pathing such that the shaders are found regardless of the project location.
const std::string ModelManager::defaultMeshName = "DefaultCubeMesh.mesh";
const std::string ModelManager::defaultMaterialName = "DefaultCubeMaterial.mat";
const std::string ModelManager::defaultShaderName = "DefaultCubeShader.shader";
const std::string ModelManager::defaultVertexShaderPath 
= "../GameEngine/DefaultShaders/DefaultVertex.vert";
const std::string ModelManager::defaultFragmentShaderPath 
= "../GameEngine/DefaultShaders/DefaultFragment.frag";
int ModelManager::materialIndex = 0;

ModelManager*
ModelManager::getInstance()
{
	if (myInstance == nullptr)
		myInstance = unique_ptr<ModelManager>(new ModelManager());

	return myInstance.get();
}

bool 
ModelManager::loadModel(const string &modelName, shared_ptr<EffectedModel> model)
{
	return store(modelName, std::move(model));
}

// Initialize neither - assume both model, material are already loaded.
bool 
ModelManager::loadModel(const string &modelName, const string &meshName, const string &materialName)
{
	return store(modelName, std::make_shared<EffectedModel>(meshName, materialName));
}

// Doesn't initialize mesh or shader, but initializes the material using the given shader
// and texture.
bool
ModelManager::loadModel(const string &modelName, const string &meshName, const string &materialName,
	const string &shaderName, const string &diffuseTexture)
{
	return store(modelName,
		std::make_shared<EffectedModel>(meshName, materialName, shaderName, diffuseTexture));
}

// Initialize mesh, but not material.
bool
ModelManager::loadModel(const string &modelName, const string &meshName, const string &materialName,
	PrimitiveType prim, const vector<Vertex> &vertices, const vector<GLuint> &indices)
{
	return store(modelName,
		std::make_shared<EffectedModel>(meshName, materialName, prim, vertices, indices));
}

// Initialize material, but not mesh.
bool
ModelManager::loadModel(const string &modelName, const string &meshName, const string &materialName,
	const string &shaderName, const string &vertexShaderPath, const string &fragmentShaderPath,
	const vector<ShaderVariable> &shaderVars, const string &diffuseTexture,
	bool printShaderLoadStatus)
{
	return store(modelName, 
		std::make_shared<EffectedModel>(meshName, materialName, shaderName,
			vertexShaderPath, fragmentShaderPath, shaderVars,
			diffuseTexture, printShaderLoadStatus));
}

// Initialization of everything.
bool 
ModelManager::loadModel(const string &modelName, const string &meshName, const string &materialName,
	const string &shaderName, PrimitiveType prim, const vector<Vertex> &vertices, 
	const vector<GLuint> &indices, const string &vertexShaderPath,
	const string &fragmentShaderPath, const vector<ShaderVariable> &shaderVars,
	const string &diffuseTexture, bool printShaderLoadStatus)
{
	return store(modelName,
		std::make_shared<EffectedModel>(meshName, materialName,
			shaderName, prim, vertices, indices, vertexShaderPath,
			fragmentShaderPath, shaderVars, diffuseTexture,
			printShaderLoadStatus));
}

bool 
ModelManager::loadCubeModel(const std::string &modelName, const std::string &diffuseTexture)
{
	vector<ShaderVariable> defaultCubeShaderVars;
	defaultCubeShaderVars.push_back(ShaderVariable(ENGINE_VAR::VERTEX_POSITION, "position"));
	defaultCubeShaderVars.push_back(ShaderVariable(ENGINE_VAR::VERTEX_UV, "uv"));
	defaultCubeShaderVars.push_back(ShaderVariable(ENGINE_VAR::SAMPLER_2D, "textureIn"));
	defaultCubeShaderVars.push_back(ShaderVariable(ENGINE_VAR::MODEL_VIEW_MATRIX, "modelViewMatrix"));
	defaultCubeShaderVars.push_back(ShaderVariable(ENGINE_VAR::PROJECTION_MATRIX, "projectionMatrix"));

	if (!MeshManager::getInstance()->get(defaultMeshName))
		MeshManager::getInstance()->loadCubeMesh(defaultMeshName);

	std::string matName = defaultMaterialName + std::to_string(materialIndex);
	if (!MaterialManager::getInstance()->get(matName))
	{
		MaterialManager::getInstance()->loadMaterial(matName,
			defaultShaderName, diffuseTexture);
		ModelManager::materialIndex++;
	}

	if (!ShaderProgramManager::getInstance()->get(defaultShaderName))
		ShaderProgramManager::getInstance()->loadShader(defaultShaderName,
			defaultVertexShaderPath, defaultFragmentShaderPath, defaultCubeShaderVars);
		
	return loadModel(modelName, defaultMeshName, matName);
}

// Load cube, shader already loaded, just need material with texture
bool
ModelManager::loadCubeModel(const string &modelName, const string &meshName, const string &materialName,
	const string &shaderName, const string &diffuseTexture)
{
	if (!MeshManager::getInstance()->get(meshName))
		MeshManager::getInstance()->loadCubeMesh(meshName);

	return loadModel(modelName, meshName, materialName, shaderName, diffuseTexture);
}

// Load cube, first time shader loading.
bool
ModelManager::loadCubeModel(const string &modelName, const string &meshName, const string &materialName,
	const string &shaderName, const string &vertexShaderPath, const string &fragmentShaderPath,
	const vector<ShaderVariable> &shaderVars, const string &diffuseTexture,
	bool printShaderLoadStatus)
{
	if (!MeshManager::getInstance()->get(meshName))
		MeshManager::getInstance()->loadCubeMesh(meshName);

	return loadModel(modelName, meshName, materialName, shaderName, 
		vertexShaderPath, fragmentShaderPath, shaderVars, diffuseTexture,
		printShaderLoadStatus);
}

/*MeshType*
ModelManager::loadModel(const string &path, string &name, ImageManager &im, int _index)
{
	// First use asset importer and linitially load the model using the following parameters.
	Assimp::Importer importer;
	scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	MeshType *mesh;
	// Polymodel = multiple meshes in single model, single model = single mesh in single model.
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
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		Vertex *verts = new Vertex[mesh->mFaces[i].mNumIndices];

		// Load vertices for the face.
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			unsigned int index = mesh->mFaces[i].mIndices[j];
			verts[j] = Vertex(Vec3(mesh->mVertices[index].x, mesh->mVertices[index].y, mesh->mVertices[index].z));

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

			if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
				GameEngine::loadImage(texturePath.C_Str(), Helper::getFullPath(path, texturePath.C_Str()));

			faces[i] = Face(Helper::numVertsToPrimType(mesh->mFaces[i].mNumIndices), verts, texturePath.C_Str());
		}
		else
			faces[i] = Face(Helper::numVertsToPrimType(mesh->mFaces[i].mNumIndices), verts);
	}
	return new Mesh(faces, mesh->mNumFaces);
}

PolyMesh*
ModelManager::buildPolyModel(const string &path, ImageManager &im)
{
	// Load each mesh using buildSingleModel method.
	vector<Mesh*> meshes;
	for (int i = 0; i < scene->mNumMeshes; i++)
		meshes.push_back(buildSingleModel(path, im, i));
	return new PolyMesh(meshes);
}*/