#include "WiiHandler.h"
#include <iostream>
#include "Game.h"

namespace MazeCrisis
{
	WiiHandler::WiiHandler(Game *game)
	{
		this->game = game;
		wiimotes = wiiuse_init(NUM_PLAYERS);
		wiiControllerConnected = connectWiimotes();
		firstCalibrationTargetSet = false;
		controllerCalibrated = false;
		//cursorSmoother = WiiCursorSmoother(8);

		cursorSmoother = std::make_unique<WiiCursorSmoother2>();
		setBottomScreenReloadPercentage(15);
	}
	
	WiiHandler::~WiiHandler()
	{
		wiiuse_cleanup(wiimotes, NUM_PLAYERS);
	}

	bool
	WiiHandler::getWiimoteConnected() const
	{
		return wiiControllerConnected;
	}

	void
	WiiHandler::setWiimoteConnected(bool value)
	{
		wiiControllerConnected = value;
	}

	int 
	WiiHandler::getBottomScreenReloadPercentage() const
	{
		return bottomScreenReloadPercentage;
	}

	void 
	WiiHandler::setBottomScreenReloadPercentage(int percent)
	{
		if (percent < 0)
			bottomScreenReloadPercentage = 0;
		else if (percent > 100)
			bottomScreenReloadPercentage = 100;
		else
			bottomScreenReloadPercentage = percent;

		unsigned int wWidth, wHeight;
		game->getWindowDimensions(wWidth, wHeight);
		float pct = bottomScreenReloadPercentage / 100.0f;
		reloadingHeightBoundary = wHeight * pct;
	}

	int 
	WiiHandler::getReloadingHeightBoundary()
	{
		return reloadingHeightBoundary;
	}

	// HARD CODED 1 PLAYER
	bool
	WiiHandler::connectWiimotes()
	{
		int found, connected;
		int wm;

		//printf("wiiuse version = %s\n", wiiuse_version());

		found = wiiuse_find(wiimotes, NUM_PLAYERS, 5);
		if (!found)
			return 0;
		connected = wiiuse_connect(wiimotes, NUM_PLAYERS);
		if (connected)
		{
			printf("Connected to %i wiimotes (of %i found).\n", connected, 
				found);
			printf("Battery level %f.\n", wiimotes[0]->battery_level);
		}
		else 
		{
			printf("Failed to connect to any wiimote.\n");
			return 0;
		}

		wiiuse_rumble(wiimotes[0], 1);
		//wiiuse_rumble(wiimotes[1], 1);

#ifndef WIN32
		usleep(200000);
#else
		Sleep(200);
#endif

		wiiuse_rumble(wiimotes[0], 0);
		//wiiuse_rumble(wiimotes[1], 0);

		unsigned int wWidth, wHeight;
		game->getWindowDimensions(wWidth, wHeight);
		float aspecRatio = (float)wHeight / (float)wWidth;

		/* enable IR and motion sensing for all wiimotes */
		for (wm = 0; wm < NUM_PLAYERS; ++wm) {
			wiiuse_set_leds(wiimotes[wm], WIIMOTE_LED_1 + wm * 0x10);
			wiiuse_motion_sensing(wiimotes[wm], 0);
			wiiuse_set_ir(wiimotes[wm], 1);
			wiiuse_set_ir_vres(wiimotes[wm], wWidth, wHeight);
			wiiuse_set_ir_sensitivity(wiimotes[wm], 5);
			wiiuse_set_ir_position(wiimotes[wm], WIIUSE_IR_ABOVE);
			//wii_use_set

			if (aspecRatio > 0.74 && aspecRatio < 0.76)
				wiiuse_set_aspect_ratio(wiimotes[wm], WIIUSE_ASPECT_4_3);
			else
				wiiuse_set_aspect_ratio(wiimotes[wm], WIIUSE_ASPECT_16_9);
		}

		//int width = wiimotes[0]->ir.vres[0];
		//int height = wiimotes[0]->ir.vres[1];

		return true;
	}

	void
	WiiHandler::update()
	{
		if (Common::gameStates.top() == GameState::CALIBRATING)
		{
			if (!firstCalibrationTargetSet)
			{
				CEGUI::Vector2<unsigned int> tmp =
					game->getUserInterface()->setTargetLocation(0);
				calibrationTargetPositions[0] = vec2(tmp.d_x, tmp.d_y);
				firstCalibrationTargetSet = true;
			}
		}

		if (wiiuse_poll(wiimotes, NUM_PLAYERS))
		{
			for (size_t i = 0; i < NUM_PLAYERS; ++i)
			{
				switch (wiimotes[i]->event) 
				{
				case WIIUSE_EVENT:
				{
					/* a generic event occured */
					wiiEventCallback(wiimotes[i]);
					break;
				}
				case WIIUSE_DISCONNECT:
				{
					// Never gets called, problem with the WiiUse library
					wiiControllerConnected = false;
					break;
				}
				default:
					break;
				}
			}
		}
	}

	void
	WiiHandler::wiiEventCallback(struct wiimote_t* wm)
	{
		vec2 smoothedPosition;
		//= cursorSmoother->update(
		//	vec2(wm->ir.x * 2, wm->ir.y * 2));

		// NOTE: To use the Wii controller, fullscreen MUST be enabled to work
		// properly
		if (controllerCalibrated)
		{
			int newX, newY;

			newX = calibrationAlphas.x * wm->ir.x
				+ calibrationBetas.x * wm->ir.y + calibrationDeltas.x;
			newY = calibrationAlphas.y * wm->ir.x
				+ calibrationBetas.y * wm->ir.y + calibrationDeltas.y;

			//smoothedPosition = cursorSmoother.addPointAndGetAverage(
			//	vec2(newX, newY));
			smoothedPosition = cursorSmoother->update(
				vec2(newX, newY));
		}
		else
		{
			//smoothedPosition = cursorSmoother.addPointAndGetAverage(
			//	vec2(wm->ir.x * 2, wm->ir.y * 2));
			smoothedPosition = cursorSmoother->update(
				vec2(wm->ir.x, wm->ir.y));
		}

		glfwSetCursorPos(game->getWindow(),
			smoothedPosition.x, smoothedPosition.y);
		game->cursorPosCallback(game->getWindow(),
			smoothedPosition.x, smoothedPosition.y);

		if (Common::gameStates.top() == GameState::CALIBRATING)
		{
			if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_B))
			{
				calibrationPoints[currentCalibrationTargetNum] =
					vec2(wm->ir.x, wm->ir.y);
				currentCalibrationTargetNum++;

				CEGUI::Vector2<unsigned int> tmp =
					game->getUserInterface()->setTargetLocation(
							currentCalibrationTargetNum);
				calibrationTargetPositions[currentCalibrationTargetNum] =
					vec2(tmp.d_x, tmp.d_y);

				if (currentCalibrationTargetNum == 3)
				{
					Common::gameStates.pop();
					currentCalibrationTargetNum = 0;
					game->getUserInterface()->closeCalibrationMenu();
					firstCalibrationTargetSet = false;
					calibrateController();
					// Reset the kalman filter
					cursorSmoother = std::make_unique<WiiCursorSmoother2>();
				}
			}
		}
		else
		{
			/*if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_ONE))
			{
				int level;
				WIIUSE_GET_IR_SENSITIVITY(wm, &level);
				wiiuse_set_ir_sensitivity(wm, level + 1);
			}

			if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_TWO))
			{
				int level;
				WIIUSE_GET_IR_SENSITIVITY(wm, &level);
				wiiuse_set_ir_sensitivity(wm, level - 1);
			}*/

			if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_B))
			{
				game->mouseHandlerCallback(game->getWindow(),
					GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0);
			}

			if (IS_RELEASED(wm, WIIMOTE_BUTTON_B))
			{
				game->mouseHandlerCallback(game->getWindow(),
					GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0);
			}

			if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_HOME))
			{
				game->keyHandlerCallback(game->getWindow(),
					GLFW_KEY_ESCAPE, 0, GLFW_PRESS, 0);
			}

			// Reloading
			if (Common::gameStates.top() == GameState::PLAYING)
			{
				if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_A) || wm->ir.y
					< getReloadingHeightBoundary())
				{
					game->keyHandlerCallback(game->getWindow(),
						GLFW_KEY_R, 0, GLFW_PRESS, 0);
				}
			}
		}
	}

	void 
	WiiHandler::windowResizedCallback(int width, int height)
	{
		if (wiiControllerConnected)
		{
			for (size_t i = 0; i < NUM_PLAYERS; ++i)
				wiiuse_set_ir_vres(wiimotes[i], width, height);
		}
	}

	// This is using an algorithm proposed by TI's Wendy Fang and Tony Chang in
	// the document "Calibration in touch-screen systems".
	// http://www.ti.com/lit/an/slyt277/slyt277.pdf
	// Variables are named after the ones mentioned in the document
	void
	WiiHandler::calibrateController()
	{
		vec3 xs(calibrationTargetPositions[0].x,
			calibrationTargetPositions[1].x,
			calibrationTargetPositions[2].x);
		vec3 ys(calibrationTargetPositions[0].y,
			calibrationTargetPositions[1].y,
			calibrationTargetPositions[2].y);
		mat3 a(calibrationPoints[0].x, calibrationPoints[0].y, 1,
			calibrationPoints[1].x, calibrationPoints[1].y, 1,
			calibrationPoints[2].x, calibrationPoints[2].y, 1);

		vec3 tmp = xs * inverse(a);
		vec3 tmp2 = ys * inverse(a);

		// Simply rewritten so it follows the document's notation.
		calibrationAlphas = vec2(tmp.x, tmp2.x);
		calibrationBetas = vec2(tmp.y, tmp2.y);
		calibrationDeltas = vec2(tmp.z, tmp2.z);

		controllerCalibrated = true;

		/*printf("Target positions: (%f, %f), (%f, %f), (%f, %f)\n",
			calibrationTargetPositions[0].x, calibrationTargetPositions[0].y,
			calibrationTargetPositions[1].x, calibrationTargetPositions[1].y,
			calibrationTargetPositions[2].x, calibrationTargetPositions[2].y);

		printf("Pointed positions: (%f, %f), (%f, %f), (%f, %f)\n",
			calibrationPoints[0].x, calibrationPoints[0].y,
			calibrationPoints[1].x, calibrationPoints[1].y,
			calibrationPoints[2].x, calibrationPoints[2].y);*/

		// Example case provided in "Calibration in touch-screen systems.
		/*vec3 xs(64, 192, 192);
		vec3 ys(384, 192, 576);
		mat3 a(678, 2169, 1,
			2807, 1327, 1,
			2629, 3367, 1);
		vec3 tmp = xs * inverse(a);
		vec3 tmp2 = ys * inverse(a);*/
	}

	/*void
	WiiHandler::cursorPosCallback(GLFWwindow* window, double x, double y)
	{
		if (wiiControllerConnected)
		{
			glfwSetCursorPos(window, )
		}
	}*/
}