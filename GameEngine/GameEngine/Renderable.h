#pragma once

#include <string>

class SceneNode;
class MatrixStack;

class Renderable
{
public:
	virtual std::string name() const = 0;
};