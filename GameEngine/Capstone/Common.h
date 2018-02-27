#pragma once

#include <stack>

namespace MazeCrisis
{
	enum GameState
	{
		MENU_MAIN,
		MENU_MAIN_QUIT, // Just quit from game to get to Main.
		MENU_LEVEL_SELECT,
		MENU_CONTROLS,
		MENU_OPTIONS,
		MENU_OPTIONS_MESSAGE,
		MENU_PAUSED,
		MENU_CALIBRATION, // Calibration window visible
		CALIBRATING,	  // Actually calibrating
		PLAYING,
		GAME_OVER,
		VICTORY,
		QUITTING
	};

	enum WEAPON
	{
		NONE = -1,
		PISTOL = 0,
		RIFLE = 1,
		SNIPER = 2
	};

	class Common
	{
	public:
		static std::stack<GameState> gameStates;
	};

	// Paths
	static const std::string GUI_PATH = "../Capstone/Assets/GUI/";
	static const std::string MODELS_PATH = "../Capstone/Assets/Models/";
	static const std::string SETTINGS_PATH = "../Capstone/Assets/Settings/";
	static const std::string SHADERS_PATH = "../Capstone/Assets/Shaders/";
	static const std::string SOUNDS_PATH = "../Capstone/Assets/Sounds/";
	static const std::string TEXTURES_PATH = "../Capstone/Assets/Textures/";
}
