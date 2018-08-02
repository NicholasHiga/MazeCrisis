#include "ShaderVariable.h"

using std::string;

/*const const string &ShaderVariable::engineVars[] = {
    "vertexPosition",
    "vertexNormal",
    "vertexUV",
    "vertexColor",
    "mvMatrix",
    "projMatrix"
};*/

ShaderVariable::ShaderVariable() 
{
    setAsVertexPositionShaderVar("");
}

ShaderVariable::ShaderVariable(ENGINE_VAR engineVar, const string &_shaderVar)
{
    uniformLocation = -1;
    if (engineVar == ENGINE_VAR::VERTEX_POSITION)
        setAsVertexPositionShaderVar(_shaderVar);
    else if (engineVar == ENGINE_VAR::VERTEX_NORMAL)
        setAsVertexNormalShaderVar(_shaderVar);
    else if (engineVar == ENGINE_VAR::VERTEX_UV)
        setAsVertexUVShaderVar(_shaderVar);
    else if (engineVar == ENGINE_VAR::VERTEX_COLOR)
        setAsVertexColorShaderVar(_shaderVar);
    else if (engineVar == ENGINE_VAR::MODEL_VIEW_MATRIX)
        setAsMatrixModelViewShaderVar(_shaderVar);
    else if (engineVar == ENGINE_VAR::PROJECTION_MATRIX)
        setAsMatrixProjectionShaderVar(_shaderVar);
    else if (engineVar == ENGINE_VAR::SAMPLER_2D)
        setAsSampler2DVar(_shaderVar);
    else if (engineVar == ENGINE_VAR::SAMPLER_CUBE)
        setAsSamplerCubeVar(_shaderVar);
}

void 
ShaderVariable::setAsVertexPositionShaderVar(const string &_shaderVarName)
{
    shaderVarType = SHADER_VAR_TYPE::ATTRIBUTE;
    //gameEngineVarName = engineVars[ENGINE_VAR::VERTEX_POSITION];
    shaderVarName = _shaderVarName;
    engineVarType = ENGINE_VAR::VERTEX_POSITION;
}

void 
ShaderVariable::setAsVertexNormalShaderVar(const string &_shaderVarName)
{
    shaderVarType = SHADER_VAR_TYPE::ATTRIBUTE;
    //gameEngineVarName = engineVars[ENGINE_VAR::VERTEX_NORMAL];
    shaderVarName = _shaderVarName;
    engineVarType = ENGINE_VAR::VERTEX_NORMAL;
}

void 
ShaderVariable::setAsVertexUVShaderVar(const string &_shaderVarName)
{
    shaderVarType = SHADER_VAR_TYPE::ATTRIBUTE;
    //gameEngineVarName = engineVars[ENGINE_VAR::VERTEX_UV];
    shaderVarName = _shaderVarName;
    engineVarType = ENGINE_VAR::VERTEX_UV;
}

void 
ShaderVariable::setAsVertexColorShaderVar(const string &_shaderVarName)
{
    shaderVarType = SHADER_VAR_TYPE::ATTRIBUTE;
    //gameEngineVarName = engineVars[ENGINE_VAR::VERTEX_COLOR];
    shaderVarName = _shaderVarName;
    engineVarType = ENGINE_VAR::VERTEX_COLOR;
}

void 
ShaderVariable::setAsMatrixModelViewShaderVar(const string &_shaderVarName)
{
    shaderVarType = SHADER_VAR_TYPE::UNIFORM;
    //gameEngineVarName = engineVars[ENGINE_VAR::MODEL_VIEW_MATRIX];
    shaderVarName = _shaderVarName;
    engineVarType = ENGINE_VAR::MODEL_VIEW_MATRIX;
}

void 
ShaderVariable::setAsMatrixProjectionShaderVar(const string &_shaderVarName)
{
    shaderVarType = SHADER_VAR_TYPE::UNIFORM;
    //gameEngineVarName = engineVars[ENGINE_VAR::PROJECTION_MATRIX];
    shaderVarName = _shaderVarName;
    engineVarType = ENGINE_VAR::PROJECTION_MATRIX;
}

void
ShaderVariable::setAsSampler2DVar(const string &_shaderVarName)
{
    shaderVarType = SHADER_VAR_TYPE::UNIFORM;
    //gameEngineVarName = engineVars[ENGINE_VAR::SAMPLER_2D];
    shaderVarName = _shaderVarName;
    engineVarType = ENGINE_VAR::SAMPLER_2D;
}

void
ShaderVariable::setAsSamplerCubeVar(const string &_shaderVarName)
{
    shaderVarType = SHADER_VAR_TYPE::UNIFORM;
    //gameEngineVarName = engineVars[ENGINE_VAR::SAMPLER_2D];
    shaderVarName = _shaderVarName;
    engineVarType = ENGINE_VAR::SAMPLER_CUBE;
}