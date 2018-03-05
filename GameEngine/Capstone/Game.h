#pragma once
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <functional>
#include "GLFW/glfw3.h"

#include "wiiuse.h"
#include "Common.h"
#include "GameEngine.h"
#include "UserInterface.h"

namespace MazeCrisis
{
	class Level;
	class UserInterface;
	class WiiHandler;

	class Game : public GameEngine
	{
	public:
		Game(GLushort windowWidth, GLushort windowHeight);
		~Game();

		void update();

		void mouseHandlerCallback(GLFWwindow* window, int button, int action, 
			int mods);
		void keyHandlerCallback(GLFWwindow* window, int key, int scancode,
			int action, int mods);
		void windowResizedCallback(GLFWwindow* window, int width, int height);
		void cursorPosCallback(GLFWwindow* window, double x, double y);
		void charCallback(GLFWwindow* window, unsigned int char_pressed);

		GLFWwindow* getWindow() const;
		WiiHandler* getWiiHandler() const;
		UserInterface* getUserInterface() const;
		void getWindowDimensions(unsigned int &refWidth,
			unsigned int &refHeight) const;
		unsigned int getWindowWidth() const;
		unsigned int getWindowHeight() const;

		void setFullscreen(bool value);

		void loadLevels();
		void reloadLevel(unsigned int level);

		static const std::string GAME_NAME;
		static const std::string MENU_MUSIC;
		static const std::string IN_GAME_MUSIC;
		static const std::string SKYBOX_PREFIX;
		static const std::string SAY_GAME_OVER;
		static const std::string GAME_OVER_TUNE;
		static const std::string VICTORY_MUSIC;

	private:	
		void initEngine();
		void initWindow();
		void initSound();

		GLFWwindow* window;
		Game* myGame;			// Needed to bypass requirement for static 
								// GLFW event handling methods.
		std::unique_ptr<WiiHandler> wiiHandler;
		GLushort windowWidth, windowHeight;
		int desktopResWidth, desktopResHeight;
		GLuint sizePerCube = 30;

		// Sound related
		bool inGameMusicStarted, gameOverTunePlayed, gameOverSaid,
			victoryMusicStarted;
		unsigned int musicFadeDuration;
		float gameOverTuneDuration, gameOverTuneStartTime;

		unsigned int currentLevel = 0;
		std::map<int, std::unique_ptr<Level>> levels;

		int lastStateLeft = GLFW_RELEASE, lastStateRight = GLFW_RELEASE;
		int lastStateUp = GLFW_RELEASE, lastStateDown = GLFW_RELEASE;
		int lastMouseState = GLFW_RELEASE;

		std::unique_ptr<UserInterface> ui;
	};
}