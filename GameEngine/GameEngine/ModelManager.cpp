#include "ModelManager.h"

#include <queue>
#include <filesystem>
#include <algorithm>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "DefaultEngineVars.h"
#include "ShaderVariable.h"
#include "ShaderProgramManager.h"
#include "MaterialManager.h"

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
    // Needed this glob to convert from relative to an absolute path.
    // Also, can't call std::replace directly on p.string().begin(),
    // p.string().end() which causes a runtime error.
    std::filesystem::path path = filepath;
    path = std::filesystem::absolute(path);
    string tmpPath = path.string();
    std::replace(tmpPath.begin(), tmpPath.end(), '\\', '/');

    const aiScene* scene = aiImportFile((const char*)tmpPath.c_str(),
        aiProcessPreset_TargetRealtime_MaxQuality);

    // Used later for loading materials, taking the model's filename off 
    // the path.
    path = tmpPath;
    path.replace_filename("");

    if (scene->mRootNode)
    {
        std::queue<aiNode*> visiting;
        visiting.push(scene->mRootNode);
        vector<SingleMesh> meshes; 
        auto matIndex = 0;

        while (!visiting.empty())
        {
            aiNode* current = visiting.front();
            for (auto i = 0; i < current->mNumMeshes; ++i)
            {

                std::vector<Vertex> verts;
                std::vector<GLuint> indices;
                const struct aiMesh* mesh =
                    scene->mMeshes[current->mMeshes[i]];

                for (auto j = 0; j < mesh->mNumVertices; ++j)
                {
                    Vertex v;
                    v.position.x = mesh->mVertices[j].x;
                    v.position.y = mesh->mVertices[j].y;
                    v.position.z = mesh->mVertices[j].z;

                    if (mesh->HasNormals())
                    {
                        v.normal.x = mesh->mNormals[j].x;
                        v.normal.y = mesh->mNormals[j].y;
                        v.normal.z = mesh->mNormals[j].z;
                    }

                    // Only checking first color set
                    if (mesh->HasVertexColors(0))
                    {
                        v.color.r = mesh->mColors[j][0].r;
                        v.color.g = mesh->mColors[j][0].g;
                        v.color.b = mesh->mColors[j][0].b;
                        if (mesh->GetNumColorChannels() == 4)
                            v.color.a = mesh->mColors[j][0].a;
                    }

                    // Not supporting 3 dimensions
                    if (mesh->HasTextureCoords(0) &&
                        *mesh->mNumUVComponents == 2)
                    {
                        v.uv.x = mesh->mTextureCoords[0][j].x;
                        v.uv.y = mesh->mTextureCoords[0][j].y;
                    }
                    verts.push_back(v);
                }

                for (auto j = 0; j < mesh->mNumFaces; ++j)
                {
                    aiFace face = mesh->mFaces[j];
                    for (auto k = 0; k < face.mNumIndices; k++)
                        indices.push_back(face.mIndices[k]);
                }

                // Model name, PrimitiveType, verts, indices, texture path
                
                if (!ShaderProgramManager::getInstance()->
                    get(defaultShaderName))
                {
                    std::vector<ShaderVariable> defaultShaderVars =
                        ShaderProgramManager::getDefaultShaderVars();
                    ShaderProgramManager::getInstance()->loadShader(
                        defaultShaderName, defaultVertexShaderPath,
                        defaultFragmentShaderPath, defaultShaderVars);
                }

                MaterialManager::getInstance()->loadAllAssimpMaterials(
                    modelName + std::to_string(matIndex), defaultShaderName,
                    path.string(), scene->mMaterials[mesh->mMaterialIndex]);

                meshes.emplace_back(SingleMesh("", 
                    modelName + std::to_string(matIndex),
                    PrimitiveType::PRIM_TRIANGLE, verts, indices));
                matIndex++;
            }

            for (auto i = 0; i < current->mNumChildren; ++i)
                visiting.push(current->mChildren[i]);
            visiting.pop();
        }
        return loadModel(modelName, std::make_shared<EffectedModel>(
            modelName, std::move(meshes)));
    }

    return false;
}

bool 
ModelManager::loadModel(const string &modelName, 
    shared_ptr<EffectedModel> model)
{
    return store(modelName, model);
}
