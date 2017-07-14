#pragma once
#include <GL/glew.h>
#include <string>
#include <map>
#include <vector>

class ShaderProgram;
class ShaderVariable;

struct Texture;

class Material
{
public:
	Material() {};
	Material(std::string shaderName);
	Material(std::string shaderName, std::string vertexShaderPath, std::string fragmentShaderPath,
		std::vector<ShaderVariable> shaderVars, bool printShaderLoadStatus = false);

	void setShader(const std::string &shaderName);	
	void setDiffuseTexture(const std::string &textureName);

	GLuint getNumTextures() { return textures.size(); }
	Texture* getDiffuseTexture() { return textures["diffuse"]; }
	GLuint getShaderID();
	ShaderProgram* getShader();

private:
	std::map<std::string, Texture*> textures; // Alpha, ambient, bumpmap, diffuse, emmisive, specular
	// map<string, vec3> colors ?
	std::string shaderName;
};