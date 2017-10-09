#pragma once
#include <vector>

#include "BasicTypes.h"
#include "MeshType.h"

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

	std::string getName();
	std::vector<Vertex> getVertices();
	std::vector<GLuint> getIndices();

	void setName(const std::string &name);
	void setVertices(std::vector<Vertex> vertices); 
	// Order of ShaderVariables matters in the ShaderProgram.

	void setIndices(std::vector<GLuint> indices);

	~SingleMesh();

protected:
	void init(const std::string &name, PrimitiveType prim,
		const std::vector<Vertex> &vertices, 
		const std::vector<GLuint> &indices);

	std::string name;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};