#include "UserInterface.h"

#include <iostream>
#include <algorithm>
#include "Game.h"
#include "WiiHandler.h"
#include "AudioManager.h"

namespace MazeCrisis
{
	using namespace std;
	using namespace CEGUI;
	using std::string;

	const string UserInterface::CROSSHAIR_ENABLED = "CrosshairEnabled";
	const string UserInterface::FULLSCREEN = "Fullscreen";
	const string UserInterface::GAME_VOLUME = "GameVolume";
	const string UserInterface::MUSIC_VOLUME = "MusicVolume";

	const string UserInterface::CROSSHAIR_IMAGE = "CrosshairImage";
	const string UserInterface::NO_CROSSHAIR = "NoCrosshair";
	const string UserInterface::PISTOL_IMAGE = "Pistol";
	const string UserInterface::RIFLE_IMAGE = "Rifle";
	const string UserInterface::WEAPON_BG_IMAGE = "WeaponBG";
	const string UserInterface::TARGET_IMAGE = "Target";
	const string UserInterface::RELOAD_MSG_IMAGE = "ReloadMessage";
	const string UserInterface::INFINITY_IMAGE = "InfinityImage";
	const string UserInterface::HEALTH_IMAGE = "HealthImage";
	const string UserInterface::AMMO_COUNT_FONT = "BroshK";
	
	const string UserInterface::GAME_OVER_SCREEN = "GameOverScreen";
	const string UserInterface::VICTORY_SCREEN = "VictoryScreen";

	const string UserInterface::MOUSE_OVER_SOUND = "mouse_over.wav";
	const string UserInterface::PLAY_BUTTON_CLICK_SOUND = "say_action.wav";

	// Needs to be called after Window has been created.
	UserInterface::UserInterface(const std::string &settingsFilePath, 
		Game* game, GLFWwindow* window)
	{
		this->settingsFilePath = settingsFilePath;
		this->game = game;
		this->window = window;

		guiRenderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
		DefaultResourceProvider* rp = static_cast<
			CEGUI::DefaultResourceProvider*>
			(CEGUI::System::getSingleton().getResourceProvider());
		guiRenderer->enableExtraStateSettings(true);

		rp->setResourceGroupDirectory("schemes", GUI_PATH + "schemes/");
		rp->setResourceGroupDirectory("imagesets", GUI_PATH + "imagesets/");
		rp->setResourceGroupDirectory("fonts", GUI_PATH + "fonts/");
		rp->setResourceGroupDirectory("layouts", GUI_PATH + "layouts/");
		rp->setResourceGroupDirectory("looknfeels", GUI_PATH + "looknfeel/");

		Scheme::setDefaultResourceGroup("schemes");
		ImageManager::setImagesetDefaultResourceGroup("imagesets");
		Font::setDefaultResourceGroup("fonts");
		WindowManager::setDefaultResourceGroup("layouts");
		WidgetLookManager::setDefaultResourceGroup("looknfeels");
		ScriptModule::setDefaultResourceGroup("lua_scripts");

		SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
		SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
		FontManager::getSingleton().createFromFile("DejaVuSans-14.font");
		FontManager::getSingleton().createFromFile("BroshK.font");
		System::getSingleton().getDefaultGUIContext().setDefaultFont(
			"DejaVuSans-14");
		System::getSingleton().getDefaultGUIContext().
			getMouseCursor().setDefaultImage("AlfiskoSkin/MouseArrow");
		System::getSingleton().getDefaultGUIContext().
			setDefaultTooltipType("TaharezLook/Tooltip");

		if (!ImageManager::getSingleton().isDefined("MainBackground"))
			ImageManager::getSingleton().addFromImageFile("MainBackground", 
				"MazeCrisisBackground.jpg");

		if (!ImageManager::getSingleton().isDefined("ControlsImage"))
			ImageManager::getSingleton().addFromImageFile("ControlsImage",
				"controls.png");

		if (!ImageManager::getSingleton().isDefined(CROSSHAIR_IMAGE))
			ImageManager::getSingleton().addFromImageFile(CROSSHAIR_IMAGE, 
				"crosshair32.png");

		if (!ImageManager::getSingleton().isDefined(NO_CROSSHAIR))
			ImageManager::getSingleton().addFromImageFile(NO_CROSSHAIR,
				"no-crosshair.png");

		if (!ImageManager::getSingleton().isDefined(PISTOL_IMAGE))
			ImageManager::getSingleton().addFromImageFile(PISTOL_IMAGE, 
				"pistol.png");

		if (!ImageManager::getSingleton().isDefined(WEAPON_BG_IMAGE))
			ImageManager::getSingleton().addFromImageFile(WEAPON_BG_IMAGE,
				"weaponBG.png");

		if (!ImageManager::getSingleton().isDefined(RIFLE_IMAGE))
			ImageManager::getSingleton().addFromImageFile(RIFLE_IMAGE,
				"rifle.png");

		if (!ImageManager::getSingleton().isDefined(TARGET_IMAGE))
			ImageManager::getSingleton().addFromImageFile(TARGET_IMAGE,
				"target.png");

		if (!ImageManager::getSingleton().isDefined(RELOAD_MSG_IMAGE))
			ImageManager::getSingleton().addFromImageFile(RELOAD_MSG_IMAGE,
				"reloadText.png");

		if (!ImageManager::getSingleton().isDefined(INFINITY_IMAGE))
			ImageManager::getSingleton().addFromImageFile(INFINITY_IMAGE,
				"infinity.png");

		if (!ImageManager::getSingleton().isDefined(HEALTH_IMAGE))
			ImageManager::getSingleton().addFromImageFile(HEALTH_IMAGE, 
				"health.png");

		if (!ImageManager::getSingleton().isDefined(GAME_OVER_SCREEN))
			ImageManager::getSingleton().addFromImageFile(GAME_OVER_SCREEN, 
				"gameOver.png");

		if (!ImageManager::getSingleton().isDefined(VICTORY_SCREEN))
			ImageManager::getSingleton().addFromImageFile(VICTORY_SCREEN,
				"levelComplete.png");

		if (!AudioManager::getInstance()->get(SOUNDS_PATH
			+ MOUSE_OVER_SOUND))
			AudioManager::getInstance()->loadSound(SOUNDS_PATH
				+ MOUSE_OVER_SOUND);

		if (!AudioManager::getInstance()->get(SOUNDS_PATH
			+ PLAY_BUTTON_CLICK_SOUND))
			AudioManager::getInstance()->loadSound(SOUNDS_PATH 
				+ PLAY_BUTTON_CLICK_SOUND);

		rootWindow = WindowManager::getSingleton().loadLayoutFromFile(
			"FullGUI.layout");
		System::getSingleton().getDefaultGUIContext().setRootWindow(rootWindow);
		mainMenuWindow = rootWindow->getChild("MainMenuWindow");
		controlsWindow = rootWindow->getChild("ControlsWindow");
		hudWindow = rootWindow->getChild("IngameHUD");
		weaponWindow = hudWindow->getChild("WeaponBG/Weapon");
		ammoCounterLabel = hudWindow->getChild("AmmoBG/AmmoText");
		reloadMessageWindow = hudWindow->getChild("ReloadMessageArea");
		invisibleFullbuffer = rootWindow->getChild("InvisibleFullscreen");
		optionsMenuWindow = invisibleFullbuffer->getChild("OptionsWindow");
		optionsMenuMessageWindow = invisibleFullbuffer->getChild(
			"InstructionsMenu");
		calibrationWindow = rootWindow->getChild("CalibrationWindow");
		calibInstructionsWindow = calibrationWindow->getChild(
			"InstructionsMenu");
		targetImageWindow = calibrationWindow->getChild("TargetWindow");
		gameOverWindow = rootWindow->getChild("GameOverBG");
		gameOverQuitButton = gameOverWindow->getChild("QuitToMainButton");
		victoryWindow = rootWindow->getChild("VictoryOverlay");

		for (size_t i = 0; i < 3; ++i)
			healthImageWindows.push_back(hudWindow->getChild(
				"HealthBG/HealthImg" + to_string(i)));

		optionsMenuWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,
			Event::Subscriber(&UserInterface::cancelSettingsButtonClick, this));

		// Main Menu Buttons
		mainMenuWindow->getChild("PlayButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(
				&UserInterface::playButtonClick, this));
		mainMenuWindow->getChild("OptionsButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(
				&UserInterface::optionsButtonClick, this));
		mainMenuWindow->getChild("ControlsButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(
				&UserInterface::controlsButtonClick, this));
		mainMenuWindow->getChild("QuitButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(
				&UserInterface::quitButtonClick, this));
		
		// Mouse Over
		mainMenuWindow->getChild("PlayButton")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
				Event::Subscriber(&UserInterface::hoverOverMainMenuButton,
					this));
		mainMenuWindow->getChild("OptionsButton")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface,
				Event::Subscriber(&UserInterface::hoverOverMainMenuButton,
					this));
		mainMenuWindow->getChild("ControlsButton")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface,
				Event::Subscriber(&UserInterface::hoverOverMainMenuButton,
					this));
		mainMenuWindow->getChild("QuitButton")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface,
				Event::Subscriber(&UserInterface::hoverOverMainMenuButton,
					this));
		
		// Options Menu
		optionsMenuWindow->getChild("CalibrateWiimote")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				Event::Subscriber(&UserInterface::calibrateWiiButtonClick,
					this));
		optionsMenuWindow->getChild("ApplySettings")->
			subscribeEvent(CEGUI::PushButton::EventClicked,
				Event::Subscriber(&UserInterface::applySettingsButtonClick, 
					this));
		optionsMenuWindow->getChild("Cancel")->
			subscribeEvent(CEGUI::PushButton::EventClicked,
				Event::Subscriber(&UserInterface::cancelSettingsButtonClick,
					this));
		optionsMenuWindow->getChild("QuitToMain")->
			subscribeEvent(CEGUI::PushButton::EventClicked,
				Event::Subscriber(&UserInterface::quitToMainButtonClick, this));
		optionsMenuMessageWindow->getChild("OkayToPromptButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				Event::Subscriber(
					&UserInterface::optionsOkayToMessageButtonClick, this));
		calibInstructionsWindow->getChild("StartCalibrationButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				Event::Subscriber(&UserInterface::startCalibrationButtonClick,
					this));
		calibInstructionsWindow->getChild("CancelCalibrationButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked,
				Event::Subscriber(&UserInterface::cancelCalibrationButtonClick,
					this));

		// Status Message for Game Over / Victory
		gameOverWindow->getChild("QuitToMainButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				Event::Subscriber(&UserInterface::quitToMainButtonClick, this));
		gameOverWindow->getChild("QuitToMainButton")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface,
				Event::Subscriber(&UserInterface::hoverOverMainMenuButton, 
					this));

		// Get/set various Option items, widgets.
		gameVolumeSlider = static_cast<CEGUI::Slider*>(optionsMenuWindow->
			getChild("GameVolumeSlider"));
		gameVolumeEditbox = static_cast<CEGUI::Editbox*>(optionsMenuWindow->
			getChild("GameVolumeValue"));
		gameVolumeSlider->subscribeEvent(CEGUI::Slider::EventValueChanged,
			Event::Subscriber(&UserInterface::gameVolumeSliderChanged, this));

		musicVolumeSlider = static_cast<CEGUI::Slider*>(optionsMenuWindow->
			getChild("MusicVolumeSlider"));
		musicVolumeEditbox = static_cast<CEGUI::Editbox*>(optionsMenuWindow->
			getChild("MusicVolumeValue"));
		musicVolumeSlider->subscribeEvent(CEGUI::Slider::EventValueChanged,
			Event::Subscriber(&UserInterface::musicVolumeSliderChanged, this));

		isCrosshairEnabledCheckbox = static_cast<CEGUI::ToggleButton*>(
			optionsMenuWindow->getChild("CrosshairEnabledCheck"));
		isFullscreenCheckbox = static_cast<CEGUI::ToggleButton*>(
			optionsMenuWindow->getChild("FullscreenEnabledCheck"));

		calibrateButton = optionsMenuWindow->getChild("CalibrateWiimote");
		quitToMainButton = optionsMenuWindow->getChild("QuitToMain");

		//optionsMenuWindow->getChild("GameVolumeValue")->
		//subscribeEvent(CEGUI::PushButton::EventClicked, 
		//	Event::Subscriber(&Game::cancelSettingsButtonClick, this));

		//pauseMenuWindow = WindowManager::getSingleton().
		// loadLayoutFromFile("PauseMenu.layout");

		//WindowManager& wmgr = WindowManager::getSingleton();
		//Window* myRoot = wmgr.createWindow("DefaultWindow", "root");
		//System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

		//FrameWindow* fWnd = static_cast<FrameWindow*>(
		//wmgr.createWindow("TaharezLook/FrameWindow", "testWindow"));
		//myRoot->addChild(fWnd);
		//// position a quarter of the way in from the top-left of parent.
		//fWnd->setPosition(UVector2(UDim(0.25f, 0.0f), UDim(0.25f, 0.0f)));
		//// set size to be half the size of the parent
		//fWnd->setSize(USize(UDim(0.5f, 0.0f), UDim(0.5f, 0.0f)));
		//fWnd->setText("Hello World!");
		//fWnd->setTitleBarEnabled(false);

		if (!loadSettings())
		{
			// Default settings in case Settings xml file not found.
			currentSettings.gameVolume = 100;
			currentSettings.musicVolume = 100;
			currentSettings.isCrosshairEnabled = true;
			currentSettings.isFullscreen = false;
		}

		setMouseCursorInGame();
		closingPromptWithOptionsMenu = false;
		isGameOverScreenFadingIn = false;
		userCanNowQuit = false;
	}

	UserInterface::~UserInterface()
	{
		CEGUI::System::destroy();
		CEGUI::OpenGL3Renderer::destroy(static_cast<CEGUI::OpenGL3Renderer&>
			(*guiRenderer));
	}
	
	void
	UserInterface::mouseHandlerCallback(GLFWwindow* window, int button, 
		int action, int mods)
	{
		if (Common::gameStates.top() != GameState::PLAYING)
		{
			if (action == GLFW_PRESS)
				CEGUI::System::getSingleton().getDefaultGUIContext()
				.injectMouseButtonDown(toCEGUIButton(button));
			else
				CEGUI::System::getSingleton().getDefaultGUIContext()
				.injectMouseButtonUp(toCEGUIButton(button));
		
			if (Common::gameStates.top() == GameState::VICTORY
				&& action == GLFW_PRESS && userCanNowQuit)
			{
				EventArgs e;
				quitToMainButtonClick(e);
			}
		}
	}

	void
	UserInterface::keyHandlerCallback(GLFWwindow* window, int key, int scancode,
		int action, int mods)
	{
		CEGUI::Key::Scan cegui_key = toCEGUIKey(key);
		if (action == GLFW_PRESS)
			CEGUI::System::getSingleton().getDefaultGUIContext()
			.injectKeyDown(cegui_key);
		else
			CEGUI::System::getSingleton().getDefaultGUIContext()
			.injectKeyUp(cegui_key);

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			if (Common::gameStates.top() == GameState::MENU_MAIN)
				Common::gameStates.push(GameState::QUITTING);
			else if (Common::gameStates.top() == GameState::MENU_LEVEL_SELECT)
				Common::gameStates.pop();
			else if (Common::gameStates.top() == GameState::MENU_OPTIONS)
				closeOptionsMenu();
			else if (Common::gameStates.top() ==
				GameState::MENU_OPTIONS_MESSAGE)
				closeOptionsMessagePrompt();
			else if (Common::gameStates.top() == GameState::MENU_CONTROLS)
			{
				Common::gameStates.pop();
				mainMenuWindow->setEnabled(true);
				controlsWindow->moveToBack();
				controlsWindow->setEnabled(false);
				controlsWindow->setVisible(false);
			}
			else if (Common::gameStates.top() == GameState::MENU_PAUSED)
				closeOptionsMenu();
			else if (Common::gameStates.top() == GameState::MENU_CALIBRATION ||
				Common::gameStates.top() == GameState::CALIBRATING)
				closeCalibrationMenu();
			else if (Common::gameStates.top() == GameState::PLAYING)
				openOptionsMenu();
		}
		else if (action == GLFW_PRESS)
		{
			if (Common::gameStates.top() == GameState::VICTORY &&
				userCanNowQuit)
			{
				EventArgs e;
				quitToMainButtonClick(e);
			}
		}
	}

	void
	UserInterface::windowResizedCallback(GLFWwindow* window, int width, 
		int height)
	{
		CEGUI::System::getSingleton().notifyDisplaySizeChanged(
			CEGUI::Sizef(static_cast<float>(width), static_cast<float>
			(height)));
	}

	void
	UserInterface::cursorPosCallback(GLFWwindow* window, double x, double y)
	{
		if (Common::gameStates.top() != GameState::PLAYING)
			CEGUI::System::getSingleton().getDefaultGUIContext()
			.injectMousePosition(x, y);
		else
			// This is to compensate for the center of the target crosshair
			// position.
			CEGUI::System::getSingleton().getDefaultGUIContext()
			.injectMousePosition(x - 16, y - 16);
	}

	void
	UserInterface::charCallback(GLFWwindow* window, unsigned int char_pressed)
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(
			char_pressed);
	}

	void
	UserInterface::update(float deltaTime)
	{
		float dTimeSeconds = deltaTime / 1000.0f;
		System::getSingleton().renderAllGUIContexts();
		System::getSingleton().injectTimePulse(dTimeSeconds);
		System::getSingleton().getDefaultGUIContext().
			injectTimePulse(dTimeSeconds);
	
		if (isReloadMessagePulsing)
		{
			float alpha = (Clock::getMilliseconds() - pulseReloadStartTime) 
				/ (pulseReloadEndTime - pulseReloadStartTime);
			bool swapFade = false;

			if (alpha >= 1.0f)
			{
				alpha = 1.0f;
				pulseReloadStartTime = Clock::getMilliseconds();
				pulseReloadEndTime = pulseReloadStartTime
					+ pulseReloadDurationHalf;
				swapFade = true;			
			}

			if (!isReloadMessageFadingOut)
				reloadMessageWindow->setAlpha(alpha);
			else
				reloadMessageWindow->setAlpha(1.0f - alpha);

			if (swapFade)
				isReloadMessageFadingOut = !isReloadMessageFadingOut;
		}

		if (isGameOverScreenFadingIn)
		{
			float alpha = (Clock::getMilliseconds() - gameOverFadeStartTime)
				/ (gameOverFadeEndTime - gameOverFadeStartTime);
			if (alpha >= 1.0f)
			{
				alpha = 1.0f;
				isGameOverScreenFadingIn = false;
				gameOverQuitButton->setEnabled(true);
			}
			gameOverWindow->setAlpha(alpha);
		}

		if (isVictoryScreenFadingIn)
		{
			float alpha = (Clock::getMilliseconds() - victoryFadeStartTime)
				/ (victoryFadeEndTime - victoryFadeStartTime);
			if (alpha >= 1.0f)
			{
				alpha = 1.0f;
				isVictoryScreenFadingIn = false;
				userCanNowQuit = true;
			}
			victoryWindow->setAlpha(alpha);
		}
	}

	void
	UserInterface::setIsCrosshairEnabled(bool value)
	{
		currentSettings.isCrosshairEnabled = value;
	}

	void
	UserInterface::setIsCrosshairEnabled(string value)
	{
		// Prob should error check.
		passStringToBool(value, "setIsCrosshairEnabled()", 
			&currentSettings.isCrosshairEnabled);
	}

	void
	UserInterface::setIsFullscreen(bool value)
	{
		currentSettings.isFullscreen = value;
	}

	void
	UserInterface::setIsFullscreen(string value)
	{
		// Prob should error check.
		passStringToBool(value, "setIsFullscreen()", 
			&currentSettings.isFullscreen);
	}

	void
	UserInterface::setGameVolume(unsigned char value)
	{
		if (value <= 0)
			currentSettings.gameVolume = 0;
		else if (value >= 100)
			currentSettings.gameVolume = 100;
		else
			currentSettings.gameVolume = value;
	}

	void
	UserInterface::setGameVolume(std::string value)
	{
		// Prob should error check.
		unsigned char tmp;
		passStringToUChar(value, "setGameVolume()", &tmp);
		setGameVolume(tmp);
	}

	void
	UserInterface::setMusicVolume(unsigned char value)
	{
		if (value <= 0)
			currentSettings.musicVolume = 0;
		else if (value >= 100)
			currentSettings.musicVolume = 100;
		else
			currentSettings.musicVolume = value;
	}

	void
	UserInterface::setMusicVolume(std::string value)
	{
		// Prob should error check.
		unsigned char tmp;
		passStringToUChar(value, "setMusicVolume()", &tmp);
		setMusicVolume(tmp);
	}

	void
	UserInterface::setWeapon(WEAPON weapon)
	{
		if (weapon == WEAPON::PISTOL)
			weaponWindow->setProperty("Image", PISTOL_IMAGE);
		else if (weapon == WEAPON::RIFLE)
			weaponWindow->setProperty("Image", RIFLE_IMAGE);
		else if (weapon == WEAPON::SNIPER)
			int x; // Sniper not implemented atm
	}

	void 
	UserInterface::setHealth(unsigned int health)
	{
		unsigned int maxHealth = 3;
		if (health <= maxHealth)
		{
			for (size_t i = 0; i < health; ++i)
				healthImageWindows[i]->setVisible(true);
			for (size_t i = health; i < maxHealth; ++i)
				healthImageWindows[i]->setVisible(false);
		}
	}

	void
	UserInterface::setAmmoCount(unsigned int currentClipAmmo, 
		unsigned int totalAmmo, bool infiniteAmmo)
	{
		CEGUI::String color = "[colour='FF000000']";

		if (!infiniteAmmo)
			ammoCounterLabel->setText(color + to_string(currentClipAmmo)
				+ " / " + to_string(totalAmmo));
		else
		{
			unsigned int fontHeight = FontManager::getSingleton()
				.get(AMMO_COUNT_FONT).getFontHeight();	
			ammoCounterLabel->setText(color + to_string(currentClipAmmo) + " / "
				+ "[vert-alignment='centre']" + "[image-size='w:" + 
				to_string(fontHeight) + " h:" + to_string(fontHeight / 2) + 
				"'][image='" + INFINITY_IMAGE + "']");
		}
	}

	Settings 
	UserInterface::getSettings()
	{
		return currentSettings;
	}

	bool 
	UserInterface::getIsCrosshairEnabled() const
	{
		return currentSettings.isCrosshairEnabled;
	}

	bool
	UserInterface::getIsFullscreen() const
	{
		return currentSettings.isFullscreen;
	}

	unsigned char
	UserInterface::getGameVolume() const
	{
		return currentSettings.gameVolume;
	}

	unsigned char
	UserInterface::getMusicVolume() const
	{
		return currentSettings.musicVolume;
	}

	float 
	UserInterface::getGameVolumeFloat() const
	{
		return (float)(currentSettings.gameVolume / 100.0f);
	}

	float 
	UserInterface::getMusicVolumeFloat() const
	{
		return (float)(currentSettings.musicVolume / 100.0f);
	}

	CEGUI::Vector2<unsigned int>
	UserInterface::setTargetLocation(TargetLocation loc)
	{
		unsigned int wWidth, wHeight;
		game->getWindowDimensions(wWidth, wHeight);
		Sizef dim = ImageManager::getSingleton()
			.get(TARGET_IMAGE).getRenderedSize();
		Vector2f relDim = getRelativeDimensions(dim.d_width, dim.d_height);

		if (loc == TargetLocation::TOP_LEFT)
			targetImageWindow->setArea(URect(UDim(0.05, 0), UDim(0.05, 0),
				UDim(0.05 + relDim.d_x, 0), UDim(0.05 + relDim.d_y, 0)));
		else if (loc == TargetLocation::TOP_RIGHT)
			targetImageWindow->setArea(URect(UDim(0.95 - relDim.d_x, 0),
				UDim(0.05, 0), UDim(0.95, 0), UDim(0.05 + relDim.d_y, 0)));
		else if (loc == TargetLocation::BOTTOM_LEFT)
			targetImageWindow->setArea(URect(UDim(0.05, 0), 
				UDim(0.95 - relDim.d_y, 0), UDim(0.05 + relDim.d_x, 0),
				UDim(0.95, 0)));
		else if (loc == TargetLocation::BOTTOM_RIGHT)
			targetImageWindow->setArea(URect(UDim(0.95 - relDim.d_x, 0), 
				UDim(0.95 - relDim.d_y, 0), UDim(0.95, 0), UDim(0.95, 0)));

		Sizef pixelSize = targetImageWindow->getPixelSize();
		Vector2f pixPosition = targetImageWindow->getPixelPosition();
		return Vector2<unsigned int>(pixPosition.d_x + (pixelSize.d_width / 2),
			pixPosition.d_y + (pixelSize.d_height / 2));
	}

	CEGUI::Vector2f 
	UserInterface::getRelativeDimensions(unsigned int width, 
		unsigned int height) const
	{
		unsigned int wWidth, wHeight;
		Vector2f relativeDim, adjustedDim;
		game->getWindowDimensions(wWidth, wHeight);
		float aspecRatio = (float)wHeight / (float)wWidth;

		if (aspecRatio > 0.74 && aspecRatio < 0.76)		
			// Don't need great accuracy, checking for 4:3
		{
			relativeDim.d_x = width / 1600.0f;	// Base dimensions 
												// we're gonna adjust with
			relativeDim.d_y = height / 1280.0f;
		}
		else
		{
			relativeDim.d_x = width / 1920.0f;	// Base dimensions we're gonna
												// adjust with
			relativeDim.d_y = height / 1080.0f;
		}

		adjustedDim.d_x = wWidth * relativeDim.d_x;
		adjustedDim.d_y = wHeight * relativeDim.d_y;
		return Vector2f(adjustedDim.d_x / wWidth, adjustedDim.d_y / wHeight);
	}

	void 
	UserInterface::pulseReloadMessage(unsigned int fadeDurationMS)
	{
		reloadMessageWindow->setVisible(true);
		reloadMessageWindow->setAlpha(1.0f);
		isReloadMessageFadingOut = true;
		isReloadMessagePulsing = true;
		pulseReloadDurationHalf = fadeDurationMS / 2;
		pulseReloadStartTime = Clock::getMilliseconds();
		pulseReloadEndTime = pulseReloadStartTime + pulseReloadDurationHalf;
	}
	
	void 
	UserInterface::stopPulsingReloadMessage()
	{
		isReloadMessagePulsing = false;
		reloadMessageWindow->setVisible(false);
	}

	void 
	UserInterface::fadeToGameOverScreen(unsigned int fadeDurationMS)
	{
		gameOverWindow->setVisible(true);
		gameOverWindow->setAlpha(0.0f);
		gameOverQuitButton->setEnabled(false);
		isGameOverScreenFadingIn = true;
		gameOverFadeStartTime = Clock::getMilliseconds();
		gameOverFadeEndTime = gameOverFadeStartTime + fadeDurationMS;
	}

	void 
	UserInterface::fadeToVictoryScreen(unsigned int fadeDurationMS)
	{
		victoryWindow->setVisible(true);
		victoryWindow->setAlpha(0.0f);
		isVictoryScreenFadingIn = true;
		victoryFadeStartTime = Clock::getMilliseconds();
		victoryFadeEndTime = victoryFadeStartTime + fadeDurationMS;
		userCanNowQuit = false;
		hudWindow->setVisible(false);
	}

	void
	UserInterface::playButtonClick(const CEGUI::EventArgs& e)
	{
		// Mouse cursor jumps for compensation of moving the image.
		Common::gameStates.push(GameState::PLAYING);
		AudioManager::getInstance()->playSound(SOUNDS_PATH 
			+ PLAY_BUTTON_CLICK_SOUND, getGameVolumeFloat());
		mainMenuWindow->setVisible(false);
		mainMenuWindow->setEnabled(false);
		hudWindow->setVisible(true);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		cursorPosCallback(window, xpos, ypos);
	}

	void
	UserInterface::optionsButtonClick(const CEGUI::EventArgs& e)
	{
		openOptionsMenu();
	}

	void
	UserInterface::controlsButtonClick(const CEGUI::EventArgs& e)
	{
		Common::gameStates.push(GameState::MENU_CONTROLS);
		mainMenuWindow->setEnabled(false);
		controlsWindow->moveToFront();
		controlsWindow->setVisible(true);
	}

	void
	UserInterface::quitButtonClick(const CEGUI::EventArgs& e)
	{
		Common::gameStates.push(GameState::QUITTING);
	}

	void 
	UserInterface::hoverOverMainMenuButton(const CEGUI::EventArgs& e)
	{
		AudioManager::getInstance()->playSound(SOUNDS_PATH
			+ MOUSE_OVER_SOUND, getGameVolumeFloat());
	}

	void
	UserInterface::applySettingsButtonClick(const CEGUI::EventArgs& e)
	{
		currentSettings.isCrosshairEnabled = 
			isCrosshairEnabledCheckbox->isSelected();
		currentSettings.isFullscreen = 
			isFullscreenCheckbox->isSelected();

		if (currentSettings.isCrosshairEnabled != 
			lastSettings.isCrosshairEnabled)
			setMouseCursorInGame();

		if (currentSettings.isFullscreen != lastSettings.isFullscreen)
			game->setFullscreen(currentSettings.isFullscreen);	

		saveSettings();
	}

	void
	UserInterface::cancelSettingsButtonClick(const CEGUI::EventArgs& e)
	{
		if (currentSettings != lastSettings)
		{
			openOptionsMessagePrompt("Settings have not been saved.");
			closingPromptWithOptionsMenu = true;
		}
		else
		{
			closeOptionsMenu();
		}

		currentSettings = lastSettings;
		AudioManager::getInstance()->setStreamVolume(getMusicVolumeFloat());
	}

	void
	UserInterface::quitToMainButtonClick(const CEGUI::EventArgs& e)
	{
		GameState oldTop = Common::gameStates.top();
		Common::gameStates.pop();
		Common::gameStates.pop();
		Common::gameStates.pop();
		Common::gameStates.push(GameState::MENU_MAIN_QUIT);
		mainMenuWindow->setVisible(true);
		mainMenuWindow->setEnabled(true);
		hudWindow->setVisible(false);
		gameOverWindow->setVisible(false);
		victoryWindow->setVisible(false);
		isGameOverScreenFadingIn = false;
		isVictoryScreenFadingIn = false;
		userCanNowQuit = false;
		if (oldTop == GameState::MENU_PAUSED)
			closeOptionsMenu();
	}

	void
	UserInterface::optionsOkayToMessageButtonClick(const CEGUI::EventArgs& e)
	{
		closeOptionsMessagePrompt();
	}

	void
	UserInterface::calibrateWiiButtonClick(const CEGUI::EventArgs& e)
	{
		WiiHandler *wiiHandler = game->getWiiHandler();
		if (wiiHandler->getWiimoteConnected())
			openCalibrationMenu();
		else
		{
			wiiHandler->setWiimoteConnected(wiiHandler->connectWiimotes());
			updateCalibrationButtonText();
			if (wiiHandler->getWiimoteConnected())
				openOptionsMessagePrompt("Wii controller connected.");
			else
				openOptionsMessagePrompt("Wii controller not found.");
		}
	}

	void
	UserInterface::startCalibrationButtonClick(const CEGUI::EventArgs& e)
	{
		Common::gameStates.push(GameState::CALIBRATING);
		calibInstructionsWindow->setVisible(false);
		calibInstructionsWindow->setEnabled(false);
		calibInstructionsWindow->moveToBack();
		targetImageWindow->setVisible(true);
		targetImageWindow->moveToFront();
		setTargetLocation(TargetLocation::TOP_LEFT);
	}

	void
	UserInterface::cancelCalibrationButtonClick(const CEGUI::EventArgs& e)
	{
		closeCalibrationMenu();
	}

	void
	UserInterface::gameVolumeSliderChanged(const CEGUI::EventArgs& e)
	{
		currentSettings.gameVolume = gameVolumeSlider->getCurrentValue()
			* 100.0f;
		gameVolumeEditbox->setText(to_string(currentSettings.gameVolume));
	}

	void
	UserInterface::musicVolumeSliderChanged(const CEGUI::EventArgs& e)
	{
		currentSettings.musicVolume = musicVolumeSlider->getCurrentValue()
			* 100.0f;
		musicVolumeEditbox->setText(to_string(currentSettings.musicVolume));
		AudioManager::getInstance()->setStreamVolume(getMusicVolumeFloat());
	}

	bool 
	UserInterface::passStringToBool(string value, string functionName,
		bool *valueToModify)
	{
		if (value == "true" || value == "false")
			istringstream(value) >> boolalpha >> *valueToModify;
		else if (value == "1" || value == "0")
			istringstream(value) >> *valueToModify;
		else
		{
			transform(value.begin(), value.end(), value.begin(), ::tolower);
			if (value == "true" || value == "false")
				istringstream(value) >> boolalpha >> *valueToModify;
			else
			{
#ifdef _DEBUG
				string err = "Invalid value " + value + " passed to " +
					functionName;
				cout << err << endl;;
				throw std::runtime_error(err);
#endif
				return false;
			}	
		}
		return true;
	}

	bool
	UserInterface::passStringToUChar(std::string value, 
		std::string functionName, unsigned char *valueToModify)
	{
		stringstream ss(value);
		int tmp;
		bool success = ss >> tmp ? true : false;

		if (success)
		{
			if (tmp <= 0)
			{
				*valueToModify = 0;
#ifdef _DEBUG
				string err = "Value " + value + 
					" casted to 0 in " + functionName;
				cout << err << endl;;
#endif
			}
			else if (tmp >= 255)
			{
				*valueToModify = 255;
#ifdef _DEBUG
				string err = "Value " + value +
					" casted to 255 in " + functionName;
				cout << err << endl;;
#endif
			}
			else
			{
				*valueToModify = tmp;
			}
			return true;
		}
		else
		{
#ifdef _DEBUG
			string err = "Invalid value " + value + " passed to " +
				functionName;
			cout << err << endl;;
			throw std::runtime_error(err);
#endif
			return false;
		}
	}

	std::string
	UserInterface::getStringFromBool(bool value)
	{
		std::stringstream tmp;
		tmp << std::boolalpha << value;
		return tmp.str();
	}

	void
	UserInterface::openOptionsMenu()
	{
		if (Common::gameStates.top() == GameState::MENU_MAIN)
		{
			Common::gameStates.push(GameState::MENU_OPTIONS);
			calibrateButton->setVisible(true);
			quitToMainButton->setVisible(false);
		}
		else if (Common::gameStates.top() == GameState::PLAYING)
		{
			Common::gameStates.push(GameState::MENU_PAUSED);
			calibrateButton->setVisible(false);
			quitToMainButton->setVisible(true);
			hudWindow->setVisible(false);
		}

		syncUIWithSettings(currentSettings);
		mainMenuWindow->setEnabled(false);
		invisibleFullbuffer->moveToFront();
		invisibleFullbuffer->setVisible(true);
		invisibleFullbuffer->setEnabled(true);
		optionsMenuWindow->moveToFront();
		optionsMenuWindow->setVisible(true);
		optionsMenuWindow->setEnabled(true);

		updateCalibrationButtonText();

		lastSettings = currentSettings;
	}

	void
	UserInterface::closeOptionsMenu()
	{
		if (Common::gameStates.top() == GameState::MENU_OPTIONS)
		{
			mainMenuWindow->setEnabled(true);
			Common::gameStates.pop();
		}
		else if (Common::gameStates.top() == GameState::MENU_PAUSED)
		{
			//gameState = GameState::PLAYING;
			Common::gameStates.pop();
			hudWindow->setVisible(true);
		}

		optionsMenuWindow->moveToBack();
		optionsMenuWindow->setEnabled(false);
		optionsMenuWindow->setVisible(false);
		invisibleFullbuffer->moveToBack();
		invisibleFullbuffer->setVisible(false);
		invisibleFullbuffer->setEnabled(false);
		closingPromptWithOptionsMenu = false;
	}

	void
	UserInterface::openOptionsMessagePrompt(string text)
	{
		// TODO: Fix the rendering bug, where mouse cursor repeats
		// when this messagebox opens in-game.
		Common::gameStates.push(GameState::MENU_OPTIONS_MESSAGE);
		optionsMenuWindow->setEnabled(false);
		invisibleFullbuffer->moveToFront();
		invisibleFullbuffer->setVisible(true);
		invisibleFullbuffer->setEnabled(true);
		optionsMenuMessageWindow->moveToFront();
		optionsMenuMessageWindow->setVisible(true);
		optionsMenuMessageWindow->setEnabled(true);
		optionsMenuMessageWindow->getChild("InstructionsLabel")->setText(text);
	}

	void 
	UserInterface::closeOptionsMessagePrompt()
	{
		optionsMenuMessageWindow->setEnabled(false);
		optionsMenuMessageWindow->setVisible(false);
		optionsMenuMessageWindow->moveToBack();
		optionsMenuWindow->setEnabled(true);
		optionsMenuWindow->moveToFront();
		Common::gameStates.pop();

		if (closingPromptWithOptionsMenu)
		{
			closeOptionsMenu();
			syncUIWithSettings(currentSettings);
		}
	}

	void
	UserInterface::openCalibrationMenu()
	{
		Common::gameStates.push(GameState::MENU_CALIBRATION);
		calibrationWindow->setVisible(true);
		calibrationWindow->setEnabled(true);
		calibrationWindow->moveToFront();
		calibInstructionsWindow->setVisible(true);
		calibInstructionsWindow->setEnabled(true);
		calibInstructionsWindow->moveToFront();
	}

	void 
	UserInterface::closeCalibrationMenu()
	{
		Common::gameStates.pop();
		calibrationWindow->setVisible(false);
		calibrationWindow->setEnabled(false);
		calibrationWindow->moveToBack();
		targetImageWindow->setVisible(false);
		optionsMenuWindow->moveToFront();
	}

	void 
	UserInterface::updateCalibrationButtonText()
	{
		if (game->getWiiHandler()->getWiimoteConnected())
			calibrateButton->setText("Calibrate Wii Controller");
		else
			calibrateButton->setText("Connect Wii Controller");
	}

	void 
	UserInterface::changeCursorVisiblityDuringCalibration(bool isVisible)
	{

	}

	bool
	UserInterface::loadSettings()
	{
		int len = 0;
		char buf[BUFSIZ] = {};
		FILE *fp;
		fopen_s(&fp, settingsFilePath.c_str(), "r");

		if (!fp) {
			fprintf(stderr, "opening %s: %s\n", settingsFilePath.c_str(), 
				strerror_s(buf, BUFSIZ, errno));
			return false;
		}

		XML_Parser parser = XML_ParserCreate(NULL);
		XML_SetUserData(parser, this);
		XML_SetElementHandler(parser, handleXML, nullptr);

		while (len = fread(buf, 1, BUFSIZ, fp))
		{
			if (0 == XML_Parse(parser, buf, len, len < BUFSIZ))
			{
				int code = XML_GetErrorCode(parser);
				const char *msg = (const char *)XML_ErrorString(
					(XML_Error)code);
				fprintf(stderr, "Parsing error code %d message %s\n", code,
					msg);
				break;
			}
		}

		// Ideally would make window after loading settings, instead
		// of making windowed window then setting it to fullscreen after.
		if (currentSettings.isFullscreen)
			game->setFullscreen(currentSettings.isFullscreen);

		fclose(fp);

		XML_ParserFree(parser);
		return true;
	}

	bool
	UserInterface::saveSettings()
	{
		std::ofstream settings;
		settings.open(settingsFilePath, 
			std::ofstream::out | std::ofstream::trunc);

		if (settings.is_open())
		{
			XMLSerializer xml(settings, 4);
			xml.openTag("Settings")
				.openTag("Property")
				.attribute("name", CROSSHAIR_ENABLED)
				.attribute("value", getStringFromBool(
					currentSettings.isCrosshairEnabled))
				.closeTag()
				.openTag("Property")
				.attribute("name", FULLSCREEN)
				.attribute("value", getStringFromBool(
					currentSettings.isFullscreen))
				.closeTag()
				.openTag("Property")
				.attribute("name", GAME_VOLUME)
				.attribute("value", to_string(
					currentSettings.gameVolume))
				.closeTag()
				.openTag("Property")
				.attribute("name", MUSIC_VOLUME)
				.attribute("value", to_string(
					currentSettings.musicVolume))
				.closeTag()
				.closeTag();
			settings.close();
			lastSettings = currentSettings;

			if (xml)
			{
				std::cout << "Settings saved successfully" << std::endl;
				openOptionsMessagePrompt("Settings saved successfully.");
				return true;
			}
			return false;
		}
		else
		{
			std::cout << "Unable to open file " << settingsFilePath 
				<< std::endl;
			return false;
		}
	}

	void 
	UserInterface::syncUIWithSettings(Settings setting)
	{
		gameVolumeSlider->setCurrentValue(setting.gameVolume / 100.0f);
		musicVolumeSlider->setCurrentValue(setting.musicVolume / 100.0f);
		gameVolumeEditbox->setText(to_string(setting.gameVolume));
		musicVolumeEditbox->setText(to_string(setting.musicVolume));
		isCrosshairEnabledCheckbox->setSelected(setting.isCrosshairEnabled);
		isFullscreenCheckbox->setSelected(setting.isFullscreen);
	}

	void 
	UserInterface::setMouseCursorInGame()
	{
		if (currentSettings.isCrosshairEnabled)
		{
			hudWindow->setProperty("MouseCursorImage", CROSSHAIR_IMAGE);
			hudWindow->getChild("WeaponBG")->setProperty("MouseCursorImage", 
				CROSSHAIR_IMAGE);
			weaponWindow->setProperty("MouseCursorImage", CROSSHAIR_IMAGE);
			hudWindow->getChild("AmmoBG")->setProperty("MouseCursorImage", 
				CROSSHAIR_IMAGE);
			ammoCounterLabel->setProperty("MouseCursorImage", CROSSHAIR_IMAGE);
		}
		else
		{
			hudWindow->setProperty("MouseCursorImage", NO_CROSSHAIR);
			hudWindow->getChild("WeaponBG")->setProperty("MouseCursorImage",
				NO_CROSSHAIR);
			weaponWindow->setProperty("MouseCursorImage", NO_CROSSHAIR);
			hudWindow->getChild("AmmoBG")->setProperty("MouseCursorImage",
				NO_CROSSHAIR);
			ammoCounterLabel->setProperty("MouseCursorImage", NO_CROSSHAIR);
		}
	}

	CEGUI::MouseButton
	UserInterface::toCEGUIButton(int button)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			return CEGUI::LeftButton;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			return CEGUI::MiddleButton;
		case GLFW_MOUSE_BUTTON_RIGHT:
			return CEGUI::RightButton;
		default:
			return CEGUI::MouseButtonCount;
		}
	}

	CEGUI::Key::Scan
	UserInterface::toCEGUIKey(int glfwKey)
	{
		switch (glfwKey)
		{
		case GLFW_KEY_ESCAPE: return CEGUI::Key::Escape;
		case GLFW_KEY_F1: return CEGUI::Key::F1;
		case GLFW_KEY_F2: return CEGUI::Key::F2;
		case GLFW_KEY_F3: return CEGUI::Key::F3;
		case GLFW_KEY_F4: return CEGUI::Key::F4;
		case GLFW_KEY_F5: return CEGUI::Key::F5;
		case GLFW_KEY_F6: return CEGUI::Key::F6;
		case GLFW_KEY_F7: return CEGUI::Key::F7;
		case GLFW_KEY_F8: return CEGUI::Key::F8;
		case GLFW_KEY_F9: return CEGUI::Key::F9;
		case GLFW_KEY_F10: return CEGUI::Key::F10;
		case GLFW_KEY_F11: return CEGUI::Key::F11;
		case GLFW_KEY_F12: return CEGUI::Key::F12;
		case GLFW_KEY_F13: return CEGUI::Key::F13;
		case GLFW_KEY_F14: return CEGUI::Key::F14;
		case GLFW_KEY_F15: return CEGUI::Key::F15;
		case GLFW_KEY_UP: return CEGUI::Key::ArrowUp;
		case GLFW_KEY_DOWN: return CEGUI::Key::ArrowDown;
		case GLFW_KEY_LEFT: return CEGUI::Key::ArrowLeft;
		case GLFW_KEY_RIGHT: return CEGUI::Key::ArrowRight;
		case GLFW_KEY_LEFT_SHIFT: return CEGUI::Key::LeftShift;
		case GLFW_KEY_RIGHT_SHIFT: return CEGUI::Key::RightShift;
		case GLFW_KEY_LEFT_CONTROL: return CEGUI::Key::LeftControl;
		case GLFW_KEY_RIGHT_CONTROL: return CEGUI::Key::RightControl;
		case GLFW_KEY_LEFT_ALT: return CEGUI::Key::LeftAlt;
		case GLFW_KEY_RIGHT_ALT: return CEGUI::Key::RightAlt;
		case GLFW_KEY_TAB: return CEGUI::Key::Tab;
		case GLFW_KEY_ENTER: return CEGUI::Key::Return;
		case GLFW_KEY_BACKSPACE: return CEGUI::Key::Backspace;
		case GLFW_KEY_INSERT: return CEGUI::Key::Insert;
		case GLFW_KEY_DELETE: return CEGUI::Key::Delete;
		case GLFW_KEY_PAGE_UP: return CEGUI::Key::PageUp;
		case GLFW_KEY_PAGE_DOWN: return CEGUI::Key::PageDown;
		case GLFW_KEY_HOME: return CEGUI::Key::Home;
		case GLFW_KEY_END: return CEGUI::Key::End;
		case GLFW_KEY_KP_ENTER: return CEGUI::Key::NumpadEnter;
		case GLFW_KEY_SPACE: return CEGUI::Key::Space;
		case 'A': return CEGUI::Key::A;
		case 'B': return CEGUI::Key::B;
		case 'C': return CEGUI::Key::C;
		case 'D': return CEGUI::Key::D;
		case 'E': return CEGUI::Key::E;
		case 'F': return CEGUI::Key::F;
		case 'G': return CEGUI::Key::G;
		case 'H': return CEGUI::Key::H;
		case 'I': return CEGUI::Key::I;
		case 'J': return CEGUI::Key::J;
		case 'K': return CEGUI::Key::K;
		case 'L': return CEGUI::Key::L;
		case 'M': return CEGUI::Key::M;
		case 'N': return CEGUI::Key::N;
		case 'O': return CEGUI::Key::O;
		case 'P': return CEGUI::Key::P;
		case 'Q': return CEGUI::Key::Q;
		case 'R': return CEGUI::Key::R;
		case 'S': return CEGUI::Key::S;
		case 'T': return CEGUI::Key::T;
		case 'U': return CEGUI::Key::U;
		case 'V': return CEGUI::Key::V;
		case 'W': return CEGUI::Key::W;
		case 'X': return CEGUI::Key::X;
		case 'Y': return CEGUI::Key::Y;
		case 'Z': return CEGUI::Key::Z;
		default: return CEGUI::Key::Unknown;
		}
	}

	void XMLCALL
	handleXML(void *userData, const char *name, const char *args[])
	{ 
		UserInterface *ui = static_cast<UserInterface*>(userData);
		//printf("%s: %s ", name, userData);
		if (strcmp(name, "Property") == 0)
		{
			string propertyName, value;
			for (size_t i = 0; args[i]; i += 2)
			{
				if (strcmp(args[i], "name") == 0)
					propertyName = args[i + 1];
				else if (strcmp(args[i], "value") == 0)
					value = args[i + 1];
			}

			if (propertyName == ui->CROSSHAIR_ENABLED)
				ui->setIsCrosshairEnabled(value);
			else if (propertyName == ui->FULLSCREEN)
				ui->setIsFullscreen(value);
			else if (propertyName == ui->GAME_VOLUME)
				ui->setGameVolume(value);
			else if (propertyName == ui->MUSIC_VOLUME)
				ui->setMusicVolume(value);
		}
	}
}