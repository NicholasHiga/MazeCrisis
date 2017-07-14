#pragma once
#include <GL/glew.h>
#include <vector>

#include "BasicTypes.h"

class MeshType
{
public:
	virtual std::vector<Vertex> getVertices() = 0;
	virtual std::vector<GLuint> getIndices() = 0;

	void setPrimitiveType(PrimitiveType prim)
	{
		try 
		{
			if (prim != PrimitiveType::PRIM_TRIANGLE && prim != PrimitiveType::PRIM_QUAD)
				throw std::invalid_argument("PrimitiveType needs to be PRIM_TRIANGLE or PRIM_QUAD");			
			this->primitiveType = prim;
		}
		catch (std::exception &e) {
			throw e;
		}
	}

	GLuint getVAO() { return vao; }
	GLuint getVBO() { return vbo; }
	GLuint getIBO() { return ibo; }
	PrimitiveType getPrimitiveType() { return primitiveType; }

protected:
	GLuint vao, vbo, ibo;			// Vertex buffer, vertex array, index buffer.													
	PrimitiveType primitiveType;	// Using only triangles, or quads as the main primitive
};