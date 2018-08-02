#pragma once

#include <string>

class SceneNode;
class MatrixStack;

class Renderable
{
public:
    virtual std::string className() const = 0;
    virtual ~Renderable() {};
};