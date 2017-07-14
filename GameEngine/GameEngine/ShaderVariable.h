#pragma once
#include <string>
#include <GL/glew.h>

// Game Engine variable names.
// It's up to the developer to create ShaderVariable types to associate
// the correct variable within the shader itself.

enum class SHADER_VAR_TYPE
{
	ATTRIBUTE,
	UNIFORM
};

// Enum to explictly make sure the developer uses one of these values.
enum ENGINE_VAR
{
	VERTEX_POSITION,
	VERTEX_NORMAL,
	VERTEX_UV,
	VERTEX_COLOR,
	MODEL_VIEW_MATRIX,
	PROJECTION_MATRIX,
	SAMPLER_2D,
	SAMPLER_CUBE
};

class ShaderVariable
{
public:
	ShaderVariable();
	ShaderVariable(ENGINE_VAR engineVar, const std::string &_shaderVar);
	//ShaderVariable(const std::string &_shaderVarName, const std::string &_gameEngineVarName, SHADER_VAR_TYPE _varType)
	//	: shaderVarName(_shaderVarName), gameEngineVarName(_gameEngineVarName), shaderVarType(_varType) { };
	ShaderVariable(const std::string &_shaderVarName, const std::string &_gameEngineVarName, SHADER_VAR_TYPE _varType)
		: shaderVarName(_shaderVarName), shaderVarType(_varType) { };

	void setAsVertexPositionShaderVar(const std::string &_shaderVarName);
	void setAsVertexNormalShaderVar(const std::string &_shaderVarName);
	void setAsVertexUVShaderVar(const std::string &_shaderVarName);
	void setAsVertexColorShaderVar(const std::string &_shaderVarName);

	void setAsMatrixModelViewShaderVar(const std::string &_shaderVarName);
	void setAsMatrixProjectionShaderVar(const std::string &_shaderVarName);
	void setAsSampler2DVar(const std::string &_shaderVarName);
	void setAsSamplerCubeVar(const std::string &_shaderVarName);

	void setUniformLocation(GLint value) { uniformLocation = value; }

	SHADER_VAR_TYPE getShaderVarType() { return shaderVarType; }
	ENGINE_VAR getEngineVarType() { return engineVarType; }
	const std::string &getShaderVarName() {	return shaderVarName; };
	//const std::string &getGameEngineVarName() { return gameEngineVarName; }
	GLint getUniformLocation() { return uniformLocation; }

private:
	std::string shaderVarName;// , gameEngineVarName;
	SHADER_VAR_TYPE shaderVarType;
	ENGINE_VAR engineVarType;
	GLint  uniformLocation;			// If -1, doesn't have an associated location.

	// These will always be the name of certain attribs/uniforms when
	// referred to within the game engine.
	//static const const std::string &engineVars[];
};
