#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "BasicTypes.h"
#include "ShaderVariable.h"

class ShaderProgram;

class Material
{
public:
    Material() {};
    Material(const std::string &name, const std::string &shaderName);
    Material(const std::string &name, const std::string &shaderName, 
        const std::string &vertexShaderPath, 
        const std::string &fragmentShaderPath, 
        std::vector<ShaderVariable> shaderVars, 
        bool printShaderLoadStatus = false);

    void setName(const std::string &name);
    void setShader(const std::string &shaderName);    

    bool addTexture(const std::string &textureName, TextureType type);
    bool addDiffuseTexture(const std::string &textureName);
    bool addSpecularTexture(const std::string &textureName);
    bool addNormalTexture(const std::string &textureName);
    bool addHeightTexture(const std::string &textureName);

    GLuint getNumTextures() const { return textures.size(); }
    std::vector<Texture*> getDiffuseTextures(); 
    std::vector<Texture*> getSpecularTextures(); 
    std::vector<Texture*> getNormalTextures(); 
    std::vector<Texture*> getHeightTextures();
    GLuint getShaderID() const;
    ShaderProgram* getShader() const;
    std::string getName() const;

private:
    std::unordered_map<TextureType, std::vector<Texture*>> textures; 
    // Alpha, ambient, bumpmap, diffuse, emmisive, specular
    // map<string, vec3> colors ?
    std::string name, shaderName;
};