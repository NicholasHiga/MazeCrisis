#include "MatrixStack.h"

using std::stack;
using glm::mat4;
using glm::mat3;

MatrixStack::MatrixStack()
{
    mat4 identity;
    projectionStack.push(identity);
    viewStack.push(identity);
    modelStack.push(identity);
}

void
MatrixStack::pushProjectionStack(mat4 m)
{
    projectionStack.push(m);
}

mat4
MatrixStack::popProjectionStack()
{
    mat4 top = projectionStack.top();
    projectionStack.pop();
    return top;
}

void 
MatrixStack::setProjectionMatrix(mat4 m)
{
    projectionStack.pop();
    projectionStack.push(m);
}

mat4
MatrixStack::getProjectionMatrix()
{
    return projectionStack.top();
}

void 
MatrixStack::pushModelStack(mat4 m)
{
    modelStack.push(m);
}

mat4
MatrixStack::popModelStack()
{
    mat4 top = modelStack.top();
    modelStack.pop();
    return top;
}

void
MatrixStack::setModelMatrix(mat4 m)
{
    modelStack.pop();
    modelStack.push(m);
}

mat4
MatrixStack::getModelMatrix()
{
    return modelStack.top();
}

void 
MatrixStack::pushViewStack(mat4 m)
{
    viewStack.push(m);
}

mat4 
MatrixStack::popViewStack()
{
    mat4 top = viewStack.top();
    viewStack.pop();
    return top;
}

void
MatrixStack::setViewMatrix(mat4 m)
{
    viewStack.pop();
    viewStack.push(m);
}

mat4
MatrixStack::getViewMatrix()
{
    return viewStack.top();
}

glm::mat4
MatrixStack::getUntranslatedViewMatrix()
{
    // Taking the current view matrix, 
    // and removing the translation.
    mat4 m = viewStack.top();
    mat3 m2 = mat3(m);
    m = mat4(m2);
    return m;
}