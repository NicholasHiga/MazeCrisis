#include "Maze.h"

#include "SceneNode.h"
#include "GameObject.h"
#include "TextureManager.h"
#include "ShaderVariable.h"
#include "ShaderProgramManager.h"

namespace MazeCrisis
{
	using std::string;
	using std::shared_ptr;
	using std::vector;
	using glm::vec3;

	Maze::Maze(GLuint sizePerCube, vec3 startPos,
		const string &meshName, const string &shaderName,
		const string &vertShaderPath, const string &fragShaderPath,
		const vector<ShaderVariable> &shaderVars, const vector<string> &textureNames,
		const vector<vector<vector<GLint>>> &textureValues)
	{
		if (!ShaderProgramManager::getInstance()->get(shaderName))
			ShaderProgramManager::getInstance()->loadShader(shaderName, vertShaderPath,
			fragShaderPath, shaderVars);

		this->shaderName = shaderName;
		//this->textures = textureValues;
		origin = startPos;
		dimensions = vec3(textureValues[0][0].size(), textureValues[0].size(), textureValues.size());
		mazeRoot = std::make_shared<SceneNode>();

		int a = textureValues.size();			//	3	// Height
		int b = textureValues[0].size();		//	2	// Length
		int c = textureValues[0][0].size();		//	1	// Width

		textures = vector<vector<vector<string>>>(textureValues.size(),
			vector<vector<string>>(textureValues[0].size(),
			vector<string>(textureValues[0][0].size())));

		for (size_t i = 0; i < textureValues.size(); i++)
		{
			for (size_t j = 0; j < textureValues[i].size(); j++)
			{
				for (size_t k = 0; k < textureValues[i][j].size(); k++)
				{
					if (textureValues[i][j][k] != 0)
						textures[i][j][k] = textureNames[textureValues[i][j][k] - 1];
					else
						textures[i][j][k] = "";
				}
			}
		}

		for (size_t i = 0; i < textures.size(); i++)
		{
			vector<vector<shared_ptr<SceneNode>>> temp1;
			for (size_t j = 0; j < textures[i].size(); j++)
			{
				vector<shared_ptr<SceneNode>> temp2;
				for (size_t k = 0; k < textures[i][j].size(); k++)
				{
					if (textures[i][j][k] != "")
					{
						if (!TextureManager::getInstance()->get(textures[i][j][k]))
						{
							TextureManager::getInstance()->loadTexture(textures[i][j][k]);
							// Making the material, and texture name the same which is the path of the texture.
							// Model will simply be the texturepath + "Cube"
							ModelManager::getInstance()->loadCubeModel(textures[i][j][k] + "Cube",
								meshName, textures[i][j][k],
								shaderName, textures[i][j][k]);
						}

						shared_ptr<SceneNode> node = std::make_shared<SceneNode>();
						gameObjects.push_back(std::make_shared<GameObject>(textures[i][j][k] + "Cube", textures[i][j][k] + "Cube"));
						gameObjects.back().get()->setIsEnabled(false);
						node->addRenderable(*gameObjects.back());
						node->setPosition(
							vec3(startPos.x + k * sizePerCube,	// 1
								startPos.y + i * sizePerCube - 2,		// 3
								startPos.z - j * sizePerCube - 4));	// 2
						node->setScale(vec3(sizePerCube, sizePerCube, sizePerCube));
						mazeRoot->appendChildNode(node);
						temp2.push_back(node);
					}

				}
				temp1.push_back(temp2);
			}
			mazeNodes.push_back(temp1);
		}
	}

	void
	Maze::changeTextureAt(vec3 cubePosition, string newTexture)
	{
		if (newTexture != "" && !TextureManager::getInstance()->get(newTexture))
			TextureManager::getInstance()->loadTexture(newTexture);

		textures[cubePosition.x][cubePosition.y][cubePosition.z] = newTexture;
		// TO DO Modify scene node as well
	}
}
