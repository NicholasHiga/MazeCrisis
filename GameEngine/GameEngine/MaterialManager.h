#pragma once
#include <memory>
#include <vector>
#include <assimp/material.h>

#include "Material.h"
#include "ResourceManager.h"

class MaterialManager : public ResourceManager<Material>
{
public:
    static MaterialManager* getInstance();

    bool loadMaterial(const std::string &materialName,
        const std::string &shaderName, const std::string &diffuseTexture = "");
    // Shader already loaded, diffuse may or may not be loaded.

    bool loadMaterial(const std::string &materialName,
        const std::string &shaderName, const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath, 
        const std::vector<ShaderVariable> &shaderVars,
        const std::string &diffuseTexturePath = "", 
        bool printShaderLoadStatus = false);
    // Load everything for the first time.

    bool loadAllAssimpMaterials(const std::string &materialName, 
        const std::string &shaderName, const std::string &materialDirectory,
        aiMaterial* material);

protected:
    void loadSpecificAssimpTextures(const std::string &materialDirectory,
        Material *material, aiMaterial* aiMaterial,
        aiTextureType textureType);

private:
    static std::unique_ptr<MaterialManager> myInstance;

    MaterialManager() {};
};