#include "Material.h"
#include <iostream>
#include "TextureManager.h"
#include "ShaderProgramManager.h"

using std::vector;

Material::Material(const std::string &name, const std::string &shaderName)
{
    setName(name);
    setShader(shaderName);
}

Material::Material(const std::string &name, const std::string &shaderName,
    const std::string &vertexShaderPath, const std::string &fragmentShaderPath,
    std::vector<ShaderVariable> shaderVars, bool printShaderLoadStatus)
{
    try
    {
        ShaderManagerLoadStatus status;
        status = ShaderProgramManager::getInstance()->loadShader(shaderName,
            vertexShaderPath, fragmentShaderPath, shaderVars);

        if (printShaderLoadStatus)
            std::cout << status.message << std::endl;

        if (status.shaderLoadStatus != OK)
            throw std::runtime_error(status.message);

        setShader(shaderName);
        setName(name);
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

void 
Material::setName(const std::string &name)
{
    this->name = name;
}

void 
Material::setShader(const std::string &shaderName)
{
    this->shaderName = shaderName;
}

bool
Material::addTexture(const std::string &textureName, TextureType type)
{
    // Was considering using an inheritance hierarchy using an ITexture, but
    // decided to stick with this instead. Yes, we can improve maintainability
    // with polymorphism but we sacrifice performance. In a real time graphics
    // app, performance matters more. I also don't expect other to work on this 
    // project so I *should* be able to cope with any maintenance changes.
    switch (type)
    {
    case TextureType::DIFFUSE:
        return addDiffuseTexture(textureName);
        break;
    case TextureType::SPECULAR:
        return addSpecularTexture(textureName);
        break;
    case TextureType::NORMAL:
        return addNormalTexture(textureName);
        break;
    case TextureType::HEIGHT:
        return addHeightTexture(textureName);
        break;
    default:
        return false;
    }
}

bool
Material::addDiffuseTexture(const std::string &textureName)
{
    if (Texture* t = TextureManager::getInstance()->get(textureName))
    {
        textures[TextureType::DIFFUSE].push_back(t);
        return true;
    }
    return false;
}

bool
Material::addSpecularTexture(const std::string &textureName)
{
    if (Texture* t = TextureManager::getInstance()->get(textureName))
    {
        textures[TextureType::SPECULAR].push_back(t);
        return true;
    }
    return false;
}

bool
Material::addNormalTexture(const std::string &textureName)
{
    if (Texture* t = TextureManager::getInstance()->get(textureName))
    {
        textures[TextureType::NORMAL].push_back(t);
        return true;
    }
    return false;
}

bool
Material::addHeightTexture(const std::string &textureName)
{
    if (Texture* t = TextureManager::getInstance()->get(textureName))
    {
        textures[TextureType::HEIGHT].push_back(t);
        return true;
    }
    return false;
}

vector<Texture*> 
Material::getDiffuseTextures()
{
    return textures[TextureType::DIFFUSE];
}

vector<Texture*>
Material::getSpecularTextures()
{
    return textures[TextureType::SPECULAR];
}

vector<Texture*>
Material::getNormalTextures()
{
    return textures[TextureType::NORMAL];
}

vector<Texture*>
Material::getHeightTextures()
{
    return textures[TextureType::DIFFUSE];
}

GLuint 
Material::getShaderID() const
{
    return ShaderProgramManager::getInstance()->get(shaderName)->getProgramID();
}

ShaderProgram*
Material::getShader() const
{
    return ShaderProgramManager::getInstance()->get(shaderName);
}

std::string
Material::getName() const
{
    return name;
}
