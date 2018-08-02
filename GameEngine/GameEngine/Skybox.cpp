#include "Skybox.h"

#include <iostream>
#include "MeshManager.h"
#include "TextureManager.h"
#include "ShaderProgramManager.h"

Skybox::Skybox(const std::string &skyboxMeshName,
    const std::string &textureName, 
    const std::string &shaderName,
    const std::string &vertexShaderPath, const std::string &fragmentShaderPath,
    const std::vector<ShaderVariable> &shaderVars,
    const std::string &negativeXTexturePath, 
    const std::string &positiveXTexturePath,
    const std::string &negativeYTexturePath, 
    const std::string &positiveYTexturePath,
    const std::string &negativeZTexturePath,
    const std::string &positiveZTexturePath,
    bool printShaderLoadStatus)
{
    try
    {
        ShaderManagerLoadStatus status;
        status.shaderLoadStatus = OK;
        if (!ShaderProgramManager::getInstance()->get(shaderName))
            status = ShaderProgramManager::getInstance()
                ->loadShader(shaderName, vertexShaderPath,
                fragmentShaderPath, shaderVars);

        if (printShaderLoadStatus)
            std::cout << status.message << std::endl;

        if (status.shaderLoadStatus != OK)
            throw std::runtime_error(status.message);

        setShaderName(shaderName);
        setMeshName(skyboxMeshName);
        setTextureName(textureName);

        if (!TextureManager::getInstance()->get(textureName))
            TextureManager::getInstance()->loadCubeMap(textureName,
                negativeXTexturePath, positiveXTexturePath,
                negativeYTexturePath, positiveYTexturePath,
                negativeZTexturePath, positiveZTexturePath);

        if (!MeshManager::getInstance()->get(skyboxMeshName))
            MeshManager::getInstance()->loadSkyboxMesh(skyboxMeshName);

        SingleMesh *m = getMesh();
        glBindVertexArray(getMesh()->getVAO());
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, getMesh()->getVBO());
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glBindVertexArray(0);
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

void 
Skybox::setMeshName(const std::string &meshName)
{
    this->meshName = meshName;
}

void
Skybox::setShaderName(const std::string &shaderName)
{
    this->shaderName = shaderName;
}

void
Skybox::setTextureName(const std::string &textureName)
{
    this->textureName = textureName;
}

ShaderProgram* 
Skybox::getShader() const
{
    return ShaderProgramManager::getInstance()->get(shaderName);
}

SingleMesh* 
Skybox::getMesh() const
{
    return MeshManager::getInstance()->get(meshName);
}

Texture* 
Skybox::getTexture() const
{
    return TextureManager::getInstance()->get(textureName);
}