#include <assimp/material.h>

#include "MaterialManager.h"
#include "TextureManager.h"
#include "ShaderVariable.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

unique_ptr<MaterialManager> MaterialManager::myInstance = nullptr;

MaterialManager*
MaterialManager::getInstance()
{
    if (myInstance == nullptr)
        myInstance = unique_ptr<MaterialManager>(new MaterialManager());

    return myInstance.get();
}

bool
MaterialManager::loadMaterial(const string &materialName, 
    const string &shaderName, const string &diffuseTexture)
{
    shared_ptr<Material> mat = std::make_shared<Material>(materialName,
        shaderName);

    if (diffuseTexture != "")
    {
        if (!TextureManager::getInstance()->get(diffuseTexture))
            TextureManager::getInstance()->loadTexture(diffuseTexture, "",
                TextureType::DIFFUSE);
        mat->addDiffuseTexture(diffuseTexture);
    }

    return store(materialName, mat);
}

bool
MaterialManager::loadMaterial(const string &materialName,
    const string &shaderName, const string &vertexShaderPath,
    const string &fragmentShaderPath, const vector<ShaderVariable> &shaderVars,
    const string &diffuseTexturePath, bool printShaderLoadStatus)
{
    shared_ptr<Material> mat = std::make_shared<Material>(materialName, 
        shaderName, vertexShaderPath, fragmentShaderPath, shaderVars,
        printShaderLoadStatus);

    if (diffuseTexturePath != "")
    {
        TextureManager::getInstance()->loadTexture(diffuseTexturePath,
            "", TextureType::DIFFUSE);
        mat->addDiffuseTexture(diffuseTexturePath);
    }

    return store(materialName, mat);
}

bool
MaterialManager::loadAllAssimpMaterials(const string &materialName, 
    const string &shaderName, const string &materialDirectory,
    aiMaterial* material)
{
    shared_ptr<Material> mat = std::make_shared<Material>(materialName,
        shaderName);

    loadSpecificAssimpTextures(materialDirectory,
        mat.get(), material, aiTextureType_DIFFUSE);
    loadSpecificAssimpTextures(materialDirectory,
        mat.get(), material, aiTextureType_SPECULAR);
    loadSpecificAssimpTextures(materialDirectory,
        mat.get(), material, aiTextureType_NORMALS);
    loadSpecificAssimpTextures(materialDirectory,
        mat.get(), material, aiTextureType_HEIGHT);

    return store(materialName, mat);
}

void
MaterialManager::loadSpecificAssimpTextures(const string &materialDirectory,
    Material *material, aiMaterial* aiMaterial, aiTextureType textureType)
{
    TextureType type = TextureManager::getInstance()->
        convertFromAssimpTextureType(textureType);
    string strType = TextureManager::getInstance()->
        convertTextureTypeToString(type);

    for (auto i = 0; i < aiMaterial->GetTextureCount(textureType); i++)
    {
        aiString textureName;
        aiMaterial->GetTexture(textureType, i, &textureName);
        string materialPath = materialDirectory + textureName.C_Str();
        string strTextureName = textureName.C_Str();
        
        // Theoretically, we can have different materials with the same name in
        // different directories which will cause this to fail.
        // TODO: Fix the problem mentioned above.
        if (!TextureManager::getInstance()->get(textureName.C_Str()))
        {
            TextureManager::getInstance()->loadTexture(materialPath,
                textureName.C_Str(), type);
            material->addTexture(textureName.C_Str(), type);
        }
    }
}