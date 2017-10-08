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
		//wiiControllerConnected = true;
	}
	
	WiiHandler::~WiiHandler()
	{
		wiiuse_cleanup(wiimotes, NUM_PLAYERS);
	}

	bool
	WiiHandler::getWiimoteConnected()
	{
		return wiiControllerConnected;
	}

	void
	WiiHandler::setWiimoteConnected(bool value)
	{
		wiiControllerConnected = value;
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
			printf("Connected to %i wiimotes (of %i found).\n", connected, found);
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
			wiiuse_set_ir_position(wiimotes[wm], WIIUSE_IR_BELOW);
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
		if (wiiuse_poll(wiimotes, NUM_PLAYERS)) {
			for (size_t i = 0; i < NUM_PLAYERS; ++i) {
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
					// Never gets called, problem with the WiiUse library.
					std::cout << "test" << std::endl;
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
		glfwSetCursorPos(game->getWindow(), wm->ir.x, wm->ir.y);
		game->cursorPosCallback(game->getWindow(), wm->ir.x, wm->ir.y);

		if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_ONE))
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
		}

		if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_B))
		{
			game->mouseHandlerCallback(game->getWindow(), GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0);
		}

		if (IS_RELEASED(wm, WIIMOTE_BUTTON_B))
		{
			game->mouseHandlerCallback(game->getWindow(), GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0);
		}

		if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_HOME))
		{
			game->keyHandlerCallback(game->getWindow(), GLFW_KEY_ESCAPE, 0, GLFW_PRESS, 0);
		}

		unsigned int wWidth, wHeight, tenPercentHeight;
		game->getWindowDimensions(wWidth, wHeight);
		tenPercentHeight = wHeight / 10;
		if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_A) || wm->ir.y < tenPercentHeight )
		{
			game->keyHandlerCallback(game->getWindow(), GLFW_KEY_R, 0, GLFW_PRESS, 0);
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

	/*void
	WiiHandler::cursorPosCallback(GLFWwindow* window, double x, double y)
	{
		if (wiiControllerConnected)
		{
			glfwSetCursorPos(window, )
		}
	}*/
}