#pragma once
#include "Maze.h"
#include "Skybox.h"
#include <string>
#include <vector>
#include <functional>
#include "GLFW/glfw3.h"

namespace MazeCrisis
{
	class Game;
	class Maze;
	class Enemy;
	class Player;
	class AutomaticCamera;

	class Level
	{
	public:
		Level(Game* game, std::string skyboxTextureLocation);
		~Level();

		void initLevel();
		void initGameObjects();
		//bool loadLevel(string levelPath);
		void update(float deltaTime);
		void mouseHandler(GLFWwindow* window, int button, int action, int mods);
		void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
		void cursorPosHandler(GLFWwindow* window, double x, double y);

	private:
		AutomaticCamera *autocam;
		Game *game;

		Maze *maze;
		Player *player;
		std::shared_ptr<Skybox> skybox;
		std::string skyboxTextureLocation;

		int lastStateLeft = GLFW_RELEASE, lastStateRight = GLFW_RELEASE;
		int lastStateUp = GLFW_RELEASE, lastStateDown = GLFW_RELEASE;
		int lastMouseState = GLFW_RELEASE;

		std::vector<float> moveSpeed;
		int lastEnemyKilled;
		//bool cameraUpdated;
		int numEnemiesTotal;
		int currentSection = 1;
		int moveStateBlackSmith = 0;
		int moveStateFooterballer = 0;
		GLuint sizePerCube = 30;

		std::vector<std::shared_ptr<Enemy>> enemies;
		std::vector<std::shared_ptr<SceneNode>> nodes;
		std::vector<std::function<void(SceneNode*, double)>> scripts;
		//Maze maze;
	};
}
