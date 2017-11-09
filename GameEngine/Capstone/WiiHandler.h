#pragma once
#include "wiiuse.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GLFW/glfw3.h"
#include "WiiCursorSmoother.h"

namespace MazeCrisis
{
	class Game;
	class UserInterface;

	using namespace glm;

	class WiiHandler
	{
	public:
		WiiHandler(Game *game);
		~WiiHandler();

		bool connectWiimotes();
		bool getWiimoteConnected() const;
		void setWiimoteConnected(bool value);

		void update();
		void wiiEventCallback(struct wiimote_t* wm);
		void windowResizedCallback(int width, int height);
		//void cursorPosCallback(GLFWwindow* window, double x, double y);

	private:
		void calibrateController();

		// Wii Related
		//GLFWwindow *window;
		//GLushort windowWidth, windowHeight;
		Game *game;
		wiimote** wiimotes = NULL;
		const int NUM_PLAYERS = 1;
		bool wiiControllerConnected = false;

		// ONLY handles 1 controller calibration.
		bool firstCalibrationTargetSet, controllerCalibrated;
		int currentCalibrationTargetNum = 0;
		vec2 calibrationPoints[3];
		vec2 calibrationTargetPositions[3];
		vec2 calibrationAlphas, calibrationBetas, calibrationDeltas;

		WiiCursorSmoother cursorSmoother;
	};
}