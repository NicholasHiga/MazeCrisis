#pragma once
#include <GL/glew.h>
#include <vector>

using std::vector;

vector<GLfloat> squarePositions = {
	-0.5f, -0.5f, 0.0f,		// Bottom left
	0.5f, -0.5f, 0.0f,		// Bottom right
	0.5f,  0.5f, 0.0f,		// Top right
	-0.5f, 0.5f, 0.0f		// Top left
};

vector<GLfloat> squareColors = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f
};

vector<GLuint> squareIndices = {
	0, 1, 2,
	0, 2, 3
};

vector<GLfloat> coloredCubePositions = {
	// front
	-0.5, -0.5,  0.5,  // Bottom left
	0.5, -0.5,  0.5,   // bottom right
	0.5,  0.5,  0.5,   // top right
	-0.5,  0.5,  0.5,  // top left
					   // back
					   -0.5, -0.5, -0.5,
					   0.5, -0.5, -0.5,
					   0.5,  0.5, -0.5,
					   -0.5,  0.5, -0.5,
};

vector<GLfloat> coloredCubeColors = {
	// front colors
	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	0.0, 0.0, 1.0,
	1.0, 0.0, 0.0,
	// back colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
};

vector<GLuint> coloredCubeIndices = {
	// front
	0, 1, 2,
	2, 3, 0,
	// top
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// bottom
	4, 0, 3,
	3, 7, 4,
	// left
	4, 5, 1,
	1, 0, 4,
	// right
	3, 2, 6,
	6, 7, 3,
};

vector<GLfloat> texturedCubeVertices =
{
	-0.5, -0.5,  0.5,
	0.5, -0.5,  0.5,
	0.5,  0.5,  0.5,
	-0.5,  0.5,  0.5,
	// top
	-0.5,  0.5,  0.5,
	0.5,  0.5,  0.5,
	0.5,  0.5, -0.5,
	-0.5,  0.5, -0.5,
	// back
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,
	-0.5,  0.5, -0.5,
	0.5,  0.5, -0.5,
	// bottom
	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, -0.5,  0.5,
	-0.5, -0.5,  0.5,
	// left
	-0.5, -0.5, -0.5,
	-0.5, -0.5,  0.5,
	-0.5,  0.5,  0.5,
	-0.5,  0.5, -0.5,
	// right
	0.5, -0.5,  0.5,
	0.5, -0.5, -0.5,
	0.5,  0.5, -0.5,
	0.5,  0.5,  0.5,
};

vector<GLfloat> texturedCubeUVs =
{
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0
};

vector<GLuint> texturedCubeIndices =
{
	// front
	0,  1,  2,
	2,  3,  0,
	// top
	4,  5,  6,
	6,  7,  4,
	// back
	8,  9, 10,
	10, 11,  8,
	// bottom
	12, 13, 14,
	14, 15, 12,
	// left
	16, 17, 18,
	18, 19, 16,
	// right
	20, 21, 22,
	22, 23, 20
};