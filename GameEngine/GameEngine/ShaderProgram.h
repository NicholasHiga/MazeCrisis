#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>

class ShaderVariable;

#define    OK                                0x00
#define    VERT_SHADER_NOT_FOUND            0x01
#define    FRAG_SHADER_NOT_FOUND            0x02
#define    VERT_SHADER_WARNING                0x04
#define    FRAG_SHADER_WARNING                0x08
#define    VERT_SHADER_COMPILATION_FAILED    0x10
#define    FRAG_SHADER_COMPILATION_FAILED    0x20
#define    LINKING_WARNINGS                0x40
#define    LINKING_FAILED                    0x80

struct ShaderLoadStatus
{
    GLushort shaderLoadStatus;
    std::string message;
};

class ShaderProgram
{
public:
    ShaderProgram() {};
    ShaderProgram(const std::string &vertexFilePath,
        const std::string &fragmentFilePath, 
        const std::vector<ShaderVariable> &shaderVars);
    ShaderLoadStatus loadShaders(const std::string &vertexFilePath, 
        const std::string &fragmentFilePath);

    GLuint getProgramID() const { return programID; }
    GLuint getModelViewMatrixID() const { return modelViewMatrixID; }
    GLuint getProjectionID() const { return projectionID; }
    std::string getVertexShaderFilePath() const { return vertexFilePath; }
    std::string getFragmentShaderFilePath() const { return fragmentFilePath; }
    std::vector<ShaderVariable> getShaderVars() const { return shaderVars; }
    std::vector<ShaderVariable> getSamplerVars() const { return samplerVars; }

    void pushShaderVar(ShaderVariable shaderVar); 
    void setShaderVars(std::vector<ShaderVariable> shaderVars);
        // Make sure the order is same as the Shader.
    void clearShaderVars() { shaderVars.clear(); samplerVars.clear(); }

private:
    GLuint programID, modelViewMatrixID, projectionID;
    std::string vertexFilePath, fragmentFilePath;
    std::vector<ShaderVariable> shaderVars;
    std::vector<ShaderVariable> samplerVars;
};    