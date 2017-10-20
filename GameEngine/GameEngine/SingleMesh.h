#pragma once
#include <vector>
#include <GL/glew.h>

#include "BasicTypes.h"
#include "MeshType.h"

class Material;
class ShaderVariable;

// It is up to the developer to make sure that the number of total vertices is
// consistent. For example, make sure that the total number of vertexPositions
// is consistent with the total number of vertexNormals.
class SingleMesh : public MeshType 
{
public:
	SingleMesh() {};
	SingleMesh(const std::string &name, PrimitiveType prim,
		const std::vector<Vertex> &vertices,
		const std::vector<GLuint> &indices);

	// Doesn't initialize mesh, shader or material, assume all are loaded
	// already.
	SingleMesh(const SingleMesh &baseMesh, const std::string &name,
		const std::string &materialName);

	// Doesn't initialize mesh or shader, but initializes the material 
	// using the given shader and texture.
	SingleMesh(const SingleMesh &baseMesh, const std::string &name, 
		const std::string &materialName, const std::string &shaderName,
		const std::string &diffuseTexture = "");

	// Initialize mesh, but not material or shader.
	SingleMesh(const std::string &name, const std::string &materialName,
		PrimitiveType prim, const std::vector<Vertex> &vertices,
		const std::vector<GLuint> &indices);

	// Initialize material and shader, but not mesh.
	SingleMesh(const SingleMesh &baseMesh, const std::string &name, 
		const std::string &materialName, const std::string &shaderName, 
		const std::string &vertexShaderPath,
		const std::string &fragmentShaderPath,
		const std::vector<ShaderVariable> &shaderVars,
		const std::string &diffuseTexture = "",
		bool printShaderLoadStatus = false);

	// Initialization of everything, mesh, material and shader.
	SingleMesh(const std::string &name, const std::string &materialName,
		const std::string &shaderName, PrimitiveType prim,
		const std::vector<Vertex> &vertices,
		const std::vector<GLuint> &indices,
		const std::string &vertexShaderPath,
		const std::string &fragmentShaderPath,
		const std::vector<ShaderVariable> &shaderVars,
		const std::string &diffuseTexture = "",
		bool printShaderLoadStatus = false);

	std::string getName() const;
	std::vector<Vertex> getVertices() const;
	std::vector<GLuint> getIndices() const;

	GLuint getVAO() const { return vao; }
	GLuint getVBO() const { return vbo; }
	GLuint getIBO() const { return ibo; }
	PrimitiveType getPrimitiveType() const { return primitiveType; }

	void setName(const std::string &name);
	void setVertices(std::vector<Vertex> vertices); 
	void setIndices(std::vector<GLuint> indices);
	// Order of ShaderVariables matters in the ShaderProgram.

	void setPrimitiveType(PrimitiveType prim);

	void addExistingMaterial(const std::string &materialName);
	const std::vector<Material*> getMaterials();

	std::string className() const { return "SingleMesh"; }

	~SingleMesh();

protected:
	void bindAttributes();
	
	void initModel(const std::string &name, const SingleMesh &baseMesh);
	void initModel(const std::string &name, PrimitiveType prim,
		const std::vector<Vertex> &vertices,
		const std::vector<GLuint> &indices);

	void initMaterial(const std::string &materialName,
		const std::string &shaderName, const std::string &vertexShaderPath,
		const std::string &fragmentShaderPath,
		const std::vector<ShaderVariable> &shaderVars,
		const std::string &texturePath = "",
		bool printShaderLoadStatus = false);

	void initMaterialWithLoadedShader(const std::string &materialName,
		const std::string &shaderName, const std::string &texturePath);

	std::string name; 
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Material*> materials;

	GLuint vao, vbo, ibo;			// Vertex buffer, vertex array,
									//	index buffer.													
	PrimitiveType primitiveType;	// Using only triangles, or quads 
									//	as the main primitive
};