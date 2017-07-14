#pragma once
#include "wiiuse.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"

namespace MazeCrisis
{
	class Game;
	class UserInterface;

	class WiiHandler
	{
	public:
		WiiHandler(Game *game);
		~WiiHandler();

		bool connectWiimotes();
		bool getWiimoteConnected();
		void setWiimoteConnected(bool value);

		void update();
		void wiiEventCallback(struct wiimote_t* wm);
		void windowResizedCallback(int width, int height);
		//void cursorPosCallback(GLFWwindow* window, double x, double y);

	private:
		// Wii Related
		//GLFWwindow *window;
		//GLushort windowWidth, windowHeight;
		Game *game;
		wiimote** wiimotes = NULL;
		const int NUM_PLAYERS = 1;
		bool wiiControllerConnected = false;
	};
}