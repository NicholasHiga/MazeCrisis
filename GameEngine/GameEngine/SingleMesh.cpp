#include "SingleMesh.h"

using std::vector;

SingleMesh::SingleMesh(PrimitiveType prim, const vector<Vertex> &vertices, const vector<GLuint> &indices)
{
	init(prim, vertices, indices);
}

void 
SingleMesh::init(PrimitiveType prim, const vector<Vertex> &vertices, const vector<GLuint> &indices)
{
	setPrimitiveType(prim);
	this->vertices = vertices;
	this->indices = indices;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	if (!indices.empty())
		glGenBuffers(1, &ibo);

	setIndices(indices);
	setVertices(vertices);
}

vector<Vertex>
SingleMesh::getVertices() 
{ 
	return vertices;
}

vector<GLuint>
SingleMesh::getIndices()
{
	return indices;
}

void
SingleMesh::setVertices(vector<Vertex> vertices)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(Vertex),
		&vertices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void 
SingleMesh::setIndices(vector<GLuint> indices)
{
	if (!indices.empty())
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indices.size() * sizeof(GLuint),
			&indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
}

SingleMesh::~SingleMesh()
{
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}