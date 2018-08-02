#pragma once
#include <glm/glm.hpp>
#include <stack>

class MatrixStack
{
public:
    MatrixStack();

    void pushProjectionStack(glm::mat4 m);
    glm::mat4 popProjectionStack();

    void setProjectionMatrix(glm::mat4 m); // Changes the top most matrix.
    glm::mat4 getProjectionMatrix();

    void pushModelStack(glm::mat4 m);
    glm::mat4 popModelStack();

    void setModelMatrix(glm::mat4 m);    // Changes the top most matrix.
    glm::mat4 getModelMatrix();

    void pushViewStack(glm::mat4 m);
    glm::mat4 popViewStack();

    void setViewMatrix(glm::mat4 m);    // Changes the top most matrix.
    glm::mat4 getViewMatrix();

    glm::mat4 getUntranslatedViewMatrix();

private:
    std::stack<glm::mat4> projectionStack;
    std::stack<glm::mat4> modelStack;
    std::stack<glm::mat4> viewStack;
};