#pragma once
#include "Common.h"
#include <expat.h>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/XMLParser.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"

namespace MazeCrisis
{
	class Game;

	struct Settings
	{
		bool isCrosshairEnabled, isFullscreen;
		unsigned char gameVolume, musicVolume;

		bool operator!=(const Settings& other) const
		{
			return (isCrosshairEnabled != other.isCrosshairEnabled)
				|| (isFullscreen != other.isFullscreen)
				|| (gameVolume != other.gameVolume)
				|| (musicVolume != other.musicVolume);
		}
	};

	class UserInterface
	{
	public:
		UserInterface(const std::string &settingsFilePath, Game *game,
			GLFWwindow* window);
		~UserInterface();

		void mouseHandlerCallback(GLFWwindow* window, int button, int action,
			int mods);
		void keyHandlerCallback(GLFWwindow* window, int key, int scancode,
			int action, int mods);
		void windowResizedCallback(GLFWwindow* window, int width, int height);
		void cursorPosCallback(GLFWwindow* window, double x, double y);
		void charCallback(GLFWwindow* window, unsigned int char_pressed);

		void update(float deltaTime);

		void setIsCrosshairEnabled(bool value);
		void setIsCrosshairEnabled(std::string value);
		void setIsFullscreen(bool value);
		void setIsFullscreen(std::string value);
		void setGameVolume(unsigned char value);
		void setGameVolume(std::string value);
		void setMusicVolume(unsigned char value);
		void setMusicVolume(std::string value);

		// Infinite ammo takes priority over total ammo, so total ammo will
		// not be shown if infinite.
		void setAmmoCount(unsigned int currentClipAmmo, unsigned int totalAmmo,
			bool infiniteAmmo = false);
		void setWeapon(WEAPON weapon);
		void setHealth(unsigned int health);

		Settings getSettings();
		bool getIsCrosshairEnabled() const;
		bool getIsFullscreen() const;
		unsigned char getGameVolume() const;
		unsigned char getMusicVolume() const;
		float getGameVolumeFloat() const;
		float getMusicVolumeFloat() const;

		CEGUI::Vector2<unsigned int> setTargetLocation(int targetNumber); 
			// Returns center of the target location.
		CEGUI::Vector2f getRelativeDimensions(unsigned int width, 
			unsigned int height) const;

		void pulseReloadMessage(unsigned int fadeDurationMS);
			// Fade duration is combination of both fade in and out.
		void stopPulsingReloadMessage();

		void fadeToGameOverScreen(unsigned int fadeDurationMS);
		void fadeToVictoryScreen(unsigned int fadeDurationMS);

		void openCalibrationMenu();
		void closeCalibrationMenu();
		void updateCalibrationButtonText();
		void changeCursorVisiblityDuringCalibration(bool isVisible);

		// Predefined constants for settings variables
		static const std::string CROSSHAIR_ENABLED;
		static const std::string FULLSCREEN;
		static const std::string GAME_VOLUME;
		static const std::string MUSIC_VOLUME;

		static const std::string CROSSHAIR_IMAGE;
		static const std::string NO_CROSSHAIR;
		static const std::string PISTOL_IMAGE;
		static const std::string RIFLE_IMAGE;
		static const std::string WEAPON_BG_IMAGE;
		static const std::string TARGET_IMAGE;
		static const std::string RELOAD_MSG_IMAGE;
		static const std::string INFINITY_IMAGE;
		static const std::string HEALTH_IMAGE;
		static const std::string AMMO_COUNT_FONT;

		static const std::string GAME_OVER_SCREEN;
		static const std::string VICTORY_SCREEN;

		static const std::string MOUSE_OVER_SOUND;
		static const std::string PLAY_BUTTON_CLICK_SOUND;

	private:
		// Buttons 
			// Main Menu Buttons
		void playButtonClick(const CEGUI::EventArgs& e);
		void optionsButtonClick(const CEGUI::EventArgs& e);
		void controlsButtonClick(const CEGUI::EventArgs& e);
		void quitButtonClick(const CEGUI::EventArgs& e);
		void hoverOverMainMenuButton(const CEGUI::EventArgs& e);

			// Options Menu Buttons
		void applySettingsButtonClick(const CEGUI::EventArgs& e);
		void cancelSettingsButtonClick(const CEGUI::EventArgs& e);
		void quitToMainButtonClick(const CEGUI::EventArgs& e);
		void optionsOkayToMessageButtonClick(const CEGUI::EventArgs& e);

			// Calibration Buttons
		void calibrateWiiButtonClick(const CEGUI::EventArgs& e);
		void startCalibrationButtonClick(const CEGUI::EventArgs& e);
		void cancelCalibrationButtonClick(const CEGUI::EventArgs& e);

		// Sliders
		void gameVolumeSliderChanged(const CEGUI::EventArgs& e);
		void musicVolumeSliderChanged(const CEGUI::EventArgs& e);

		// Returns true on successful cast, false on failure.
		bool passStringToBool(std::string value, std::string functionName,
			bool *valueToModify);

		// Returns true on successful cast, false on failure.
		bool passStringToUChar(std::string value, std::string functionName,
			unsigned char *valueToModify);
		std::string getStringFromBool(bool value);

		void openOptionsMenu();
		void closeOptionsMenu();

		void openOptionsMessagePrompt(std::string text);
		void closeOptionsMessagePrompt();

		//void showGameHUD();
		//void hideGameHUD();

		bool loadSettings();
		bool saveSettings();
		void syncUIWithSettings(Settings setting);
		void setMouseCursorInGame(); // Set mouse cursor depending on 
									 // current setting.

		CEGUI::MouseButton toCEGUIButton(int button);
		CEGUI::Key::Scan toCEGUIKey(int glfwKey);

		CEGUI::OpenGL3Renderer *guiRenderer;

		// TODO: Determine if calibration is actually needed, remove
		// appropriate code if not required.
		// Main menu windows.
		CEGUI::Window *rootWindow, *mainMenuWindow,
			*optionsMenuWindow, *controlsWindow, *targetImageWindow,
			*calibrationWindow, *calibInstructionsWindow,
			*optionsMenuMessageWindow;

		std::vector<CEGUI::Window*> healthImageWindows;

		// In-game windows
		CEGUI::Window *invisibleFullbuffer, *hudWindow,
			*weaponWindow, *reloadMessageWindow, *ammoCounterLabel;

		// Game Over / Victory Status Windows
		CEGUI::Window *gameOverWindow, *gameOverQuitButton, *victoryWindow;
		
		// Buttons
		CEGUI::Window *calibrateButton, *quitToMainButton,
			*cancelCalibrationButton, *optionsOkayToMessageButton;

		// Widgets
		CEGUI::Slider *gameVolumeSlider, *musicVolumeSlider;
		CEGUI::Editbox *gameVolumeEditbox, *musicVolumeEditbox;
		CEGUI::ToggleButton  *isCrosshairEnabledCheckbox, *isFullscreenCheckbox;

		std::string settingsFilePath;

		// lastSettings = Settings before anything is applied.
		Settings lastSettings, currentSettings;

		Game* game; // Mainly needed for Fullscreen
		GLFWwindow* window; // Needed for fixing mouse jitter on play.
		bool closingPromptWithOptionsMenu;

		// For pulsing reload message in and out.
		bool isReloadMessagePulsing, isReloadMessageFadingOut;
		double pulseReloadStartTime, pulseReloadEndTime;
		unsigned int pulseReloadDurationHalf;

		// For fading game over screen in.
		bool isGameOverScreenFadingIn;
		double gameOverFadeStartTime, gameOverFadeEndTime;

		// For fading victory screen in.
		bool isVictoryScreenFadingIn, userCanNowQuit;
		double victoryFadeStartTime, victoryFadeEndTime;
	};

	// "Event" handler any time a new element is passed in.
	static void XMLCALL 
		handleXML(void *userData, const char *name, const char *args[]); 
}