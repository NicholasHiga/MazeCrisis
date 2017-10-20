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
	Material(const std::string &name, const std::string &shaderName);
	Material(const std::string &name, const std::string &shaderName, 
		const std::string &vertexShaderPath, 
		const std::string &fragmentShaderPath, 
		std::vector<ShaderVariable> shaderVars, 
		bool printShaderLoadStatus = false);

	void setName(const std::string &name);
	void setShader(const std::string &shaderName);	
	void setDiffuseTexture(const std::string &textureName);

	GLuint getNumTextures() const { return textures.size(); }
	Texture* getDiffuseTexture() { return textures["diffuse"]; }
	GLuint getShaderID() const;
	ShaderProgram* getShader() const;
	std::string getName() const;

private:
	std::map<std::string, Texture*> textures; 
	// Alpha, ambient, bumpmap, diffuse, emmisive, specular
	// map<string, vec3> colors ?
	std::string name, shaderName;
};