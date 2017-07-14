#include "Face.h"

/*Face::Face(PrimitiveType _primType, Vertex *_vertices, string _textureName, int _numVertices)
{
    primType = _primType;
    vertices = _vertices;

    if (primType == PrimitiveType::PRIM_POINT)
        numVertices = 1;
    else if (primType == PrimitiveType::PRIM_LINE)
        numVertices = 2;
    else if (primType == PrimitiveType::PRIM_TRIANGLE)
        numVertices = 3;
    else if (primType == PrimitiveType::PRIM_QUAD)
        numVertices = 4;
    else // PrimitiveType::TriangleFan
        numVertices = _numVertices;

	textureName = _textureName;

	isTextured = textureName == "" ? false : true;
}

int 
Face::NumVertices()
{
    return numVertices;
}

PrimitiveType 
Face::PrimType()
{
    return primType;
}

Vertex* 
Face::Vertices()
{
    return vertices;
}

string 
Face::TextureName()
{
	return textureName;
}


bool
Face::IsTextured()
{
	return isTextured;
}

void 
Face::NumVertices(int val)
{
    numVertices = val;
}

void 
Face::PrimType(PrimitiveType val)
{
    primType = val;
}

void 
Face::Vertices(Vertex &val)
{
    vertices = &val;
}

void
Face::TextureName(string name)
{
	textureName = name;
}
*/