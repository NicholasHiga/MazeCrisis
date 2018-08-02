#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>

#include "MeshManager.h"

class GameObject;
class SceneNode;

namespace MazeCrisis
{
    class Maze
    {
    public:
        /// Start pos refers to the bottom left, closest to the camera.
        /// Dimensions = (length, width, height)
        /// Note that if in the textures std::vector, the std::string is ""
        /// that means there is no block there.
        Maze(GLuint sizePerCube, glm::vec3 startPos,
            const std::string &meshName, const std::string &shaderName,
            const std::string &vertShaderPath, 
            const std::string &fragShaderPath,
            const std::vector<ShaderVariable> &shaderVars, 
            const std::vector<std::string> &textureNames,
            const std::vector<std::vector<std::vector<GLint>>> &textureValues);

        std::shared_ptr<SceneNode> getMazeRoot() const { return mazeRoot; }
        std::vector<std::vector<std::vector
            <std::shared_ptr<SceneNode>>>>* getMazeNodes()
            { return &mazeNodes; }
        GLuint getSizePerCube() const { return sizePerCube; }

        void changeTextureAt(glm::vec3 cubePosition, std::string newTexture);

    private:
        std::shared_ptr<SceneNode> mazeRoot;
        std::vector<std::vector<std::vector<std::shared_ptr<SceneNode>>>> 
            mazeNodes;
        std::vector<std::vector<std::vector<std::string>>> textures;
        std::vector<std::shared_ptr<GameObject>> gameObjects;

        GLuint sizePerCube;
        glm::vec3 origin, dimensions;
        std::string shaderName; // Assumes only 1 shader for the whole maze.
    };
}