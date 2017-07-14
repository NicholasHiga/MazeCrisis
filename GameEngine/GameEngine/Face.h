/*#ifndef __FACE__H__
#define __FACE__H__

#include "BasicTypes.h"
#include <string>

using std::string;

class Face
{
public:
    Face() {};
    Face(PrimitiveType primType, Vertex *vertices, string textureName = "", int numVertices = 0);
    ~Face() {};

    int NumVertices();
    PrimitiveType PrimType();
    Vertex* Vertices();
	string TextureName();
	bool IsTextured();

    void NumVertices(int val);
    void PrimType(PrimitiveType val);
    void Vertices(Vertex &val);
	void TextureName(string name);

private:
    int numVertices;
    Vertex *vertices;
    PrimitiveType primType;
	string textureName;
	bool isTextured;
};

#endif*/