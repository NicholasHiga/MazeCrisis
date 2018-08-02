#include "Game.h"

#include <iostream>
#include <filesystem>
#include "Debug.h"
#include "Level.h"
#include "RayQuery.h"
#include "WiiHandler.h"
#include "AutomaticCamera.h"

namespace MazeCrisis
{
    using glm::vec3;
    using std::string;
    using std::vector;
    using std::shared_ptr;

    const string Game::GAME_NAME = "Maze Crisis";
    const string Game::MENU_MUSIC = "menu_music.wav";
    const string Game::IN_GAME_MUSIC = "ingame_music.wav";
    const string Game::SKYBOX_PREFIX = "hw_lagoon/lagoon";
    const string Game::SAY_GAME_OVER = "say_game_over.wav";
    const string Game::GAME_OVER_TUNE = "game_over.wav";
    const string Game::VICTORY_MUSIC = "victory_music.wav";

    Game::Game(GLushort windowWidth, GLushort windowHeight)
        : GameEngine(*OpenGLRenderer::getInstance(), windowWidth, windowHeight)
    {
        glEnable(GL_DEBUG_OUTPUT);
        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;
        myGame = this;
        try
        {
            wiiHandler = std::make_unique<WiiHandler>(this);
            initWindow();
            ui = std::make_unique<UserInterface>(SETTINGS_PATH + "Settings.xml",
                this, window);
            initEngine();
            loadLevels();
            Common::gameStates.push(GameState::MENU_MAIN);
            initSound();
            std::cout << "Current path is " << std::filesystem::current_path() << '\n';

#ifdef _DEBUG
            if (GLEW_ARB_debug_output) {
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
                glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE,
                    GL_DONT_CARE, 0, NULL, GL_TRUE);
                glDebugMessageCallbackARB((GLDEBUGPROCARB)ETB_GL_ERROR_CALLBACK,
                    NULL);
            }
#endif
        }
        catch (std::exception &e)
        {
            throw e;
        }
    }
        
    Game::~Game()
    {
        onEnd();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void
    Game::update()
    {
        do {
            float deltaTime = Clock::getMilliseconds() - numMilliseconds;
            preRender(deltaTime);
            numMilliseconds = Clock::getMilliseconds();
            if (Common::gameStates.top() == GameState::PLAYING)
            {
                if (!inGameMusicStarted)
                {
                    inGameMusicStarted = true;
                    AudioManager::getInstance()->playNextStream(SOUNDS_PATH
                        + IN_GAME_MUSIC, musicFadeDuration,
                        ui->getMusicVolumeFloat());
                    // AudioManager::getInstance()->
                    // fadeStreamOut(musicFadeDuration);
                    //AudioManager::getInstance()->playStreamFadeIn(
                    // SOUNDS_PATH + IN_GAME_MUSIC, musicFadeDuration, 
                    // ui->getMusicVolumeFloat());
                }

                render();
                levels[currentLevel]->update(deltaTime);
            }
            else if (Common::gameStates.top() == GameState::MENU_MAIN_QUIT)
            {
                // Quitting the game from in-game. Reloading the level for one 
                // frame.
                Common::gameStates.pop();
                Common::gameStates.push(GameState::MENU_MAIN);
                reloadLevel(currentLevel);
                if (AudioManager::getInstance()->isStreamPlaying)
                    AudioManager::getInstance()->playNextStream(
                        SOUNDS_PATH + MENU_MUSIC, musicFadeDuration,
                        ui->getMusicVolumeFloat());
                else
                    AudioManager::getInstance()->playStream(
                        SOUNDS_PATH + MENU_MUSIC, ui->getMusicVolumeFloat());
                inGameMusicStarted = false;
                gameOverSaid = false;
                gameOverTunePlayed = false;
                victoryMusicStarted = false;
            }
            else if (Common::gameStates.top() == GameState::MENU_PAUSED)
            {
                render();
            }
            else if (Common::gameStates.top() == GameState::GAME_OVER)
            {
                render();
                if (!gameOverTunePlayed)
                {
                    AudioManager::getInstance()->fadeStreamOut(
                        musicFadeDuration / 2);
                    gameOverTuneStartTime = Clock::getMilliseconds();
                    AudioManager::getInstance()->playSound(
                        SOUNDS_PATH + GAME_OVER_TUNE,
                        ui->getMusicVolumeFloat());
                    gameOverTunePlayed = true;
                    ui->fadeToGameOverScreen(gameOverTuneDuration);
                }

                if (!gameOverSaid &&
                    Clock::getMilliseconds() - gameOverTuneStartTime >
                    gameOverTuneDuration)
                {
                    AudioManager::getInstance()->playSound(
                        SOUNDS_PATH + SAY_GAME_OVER, ui->getGameVolumeFloat());
                    gameOverSaid = true;
                }
            }
            else if (Common::gameStates.top() == GameState::VICTORY)
            {
                render();

                if (!victoryMusicStarted)
                {
                    AudioManager::getInstance()->fadeStreamOut(
                        musicFadeDuration / 2);
                    ui->fadeToVictoryScreen(2000);
                    victoryMusicStarted = true;
                    AudioManager::getInstance()->playStream(
                        SOUNDS_PATH + VICTORY_MUSIC, ui->getMusicVolumeFloat());
                }
            }

            GameEngine::update(deltaTime);
            ui->update(deltaTime);

            //if (wiiHandler->getWiimoteConnected())
                wiiHandler->update(deltaTime);

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
            postRender();

        } // Check if the ESC key was pressed or the window was closed
        while (glfwWindowShouldClose(window) == 0 &&
            Common::gameStates.top() != GameState::QUITTING);
    }


    void
    Game::mouseHandlerCallback(GLFWwindow* window, int button, int action,
        int mods)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        Ray r = RayQuery::getRayFromMouseClick((int)xpos, (int)ypos);
        levels[currentLevel]->mouseHandler(window, button, action, mods);
        ui->mouseHandlerCallback(window, button, action, mods);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS
            && lastMouseState == GLFW_RELEASE)
            lastMouseState = GLFW_PRESS;

        if (action == GLFW_RELEASE)
            lastMouseState = GLFW_RELEASE;
    }

    void
    Game::keyHandlerCallback(GLFWwindow* window, int key, int scancode, 
        int action, int mods)
    {
        ui->keyHandlerCallback(window, key, scancode, action, mods);
        levels[currentLevel].get()->keyHandler(window, key, scancode, action,
            mods);
    }

    void
    Game::windowResizedCallback(GLFWwindow* window, int width, int height)
    {
        windowWidth = width;
        windowHeight = height;
        renderer->setWindowSize(width, height, getCamera());
        ui->windowResizedCallback(window, width, height);
        wiiHandler->windowResizedCallback(width, height);
    }

    void
    Game::cursorPosCallback(GLFWwindow* window, double x, double y)
    {
        //wiiHandler->cursorPosCallback(window, x, y);
        ui->cursorPosCallback(window, x, y);
        levels[currentLevel]->cursorPosHandler(window, x, y);
    }

    void
    Game::charCallback(GLFWwindow* window, unsigned int char_pressed)
    {
        ui->charCallback(window, char_pressed);
    }

    GLFWwindow*
    Game::getWindow() const
    {
        return window;
    }

    WiiHandler*
    Game::getWiiHandler() const
    {
        return wiiHandler.get();
    }

    UserInterface* 
    Game::getUserInterface() const
    {
        return ui.get();
    }

    void
    Game::getWindowDimensions(unsigned int &refWidth, unsigned int &refHeight)
        const
    {
        refWidth = windowWidth;
        refHeight = windowHeight;
    }

    unsigned int
    Game::getWindowWidth() const
    {
        return windowWidth;
    }

    unsigned int 
    Game::getWindowHeight() const
    {
        return windowHeight;
    }

    void
    Game::setFullscreen(bool isFullscreen)
    {
        if (isFullscreen)
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0,
                windowWidth, windowHeight, GLFW_DONT_CARE);
        else
        {
            glfwSetWindowMonitor(window, NULL,
                desktopResWidth / 2 - windowWidth / 2,
                desktopResHeight / 2 - windowHeight / 2,
                windowWidth, windowHeight, GLFW_DONT_CARE);
        }
    }

    void
    Game::loadLevels()
    {
        levels[0] = std::make_unique<Level>(this, SKYBOX_PREFIX);
    }

    void
    Game::reloadLevel(unsigned int level)
    {
        levels.erase(level);
        levels[level] = std::make_unique<Level>(this, SKYBOX_PREFIX);
        ui->stopPulsingReloadMessage();
    }

    void
    Game::initEngine()
    {
        onStart();
        renderer = OpenGLRenderer::getInstance();

        vec3 startingBlock(1, 1, 1);
        vec3 cameraStart(startingBlock.x * sizePerCube,
            startingBlock.y * sizePerCube,
            startingBlock.z * (GLint)sizePerCube * -1);
        camera = std::make_unique<AutomaticCamera>(ui.get(), windowWidth,
            windowHeight);
        renderer->setWindowSize(windowWidth, windowHeight, getCamera());
    }

    void
    Game::initWindow()
    {
        try
        {
            // Initialise GLFW
            if (!glfwInit())
                throw std::runtime_error("Failed to initialize GLFW\n");

            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
                // To make MacOS happy; should not be needed
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

            window = glfwCreateWindow(windowWidth, windowHeight,
                GAME_NAME.c_str(), NULL, NULL);
            
            const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            desktopResWidth = mode->width;
            desktopResHeight = mode->height;
            glfwSetWindowMonitor(window, NULL,
                desktopResWidth / 2 - windowWidth / 2,
                desktopResHeight / 2 - windowHeight / 2, windowWidth, 
                windowHeight, GLFW_DONT_CARE);

            if (window == NULL) {
                glfwTerminate();
                throw std::runtime_error("Failed to open GLFW window. \n");
            }
            glfwMakeContextCurrent(window);

            glewExperimental = GL_TRUE;
            // Initialize GLEW
            if (glewInit() != GLEW_OK) {
                glfwTerminate();
                throw std::runtime_error("Failed to initialize GLEW\n");
            }

            // Ensure we can capture the escape key being pressed below
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
            // Hide the mouse and enable unlimited mouvement
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            glfwSetWindowUserPointer(window, myGame);

            // These functions required to link to non-static functions.
            auto func = [](GLFWwindow* window, int button, int action, int mods)
            {
                static_cast<Game*>
                    (glfwGetWindowUserPointer(window))->mouseHandlerCallback(
                        window, button, action, mods);
            };

            auto func2 = [](GLFWwindow* window, int key, int scancode,
                int action, int mods)
            {
                static_cast<Game*>
                    (glfwGetWindowUserPointer(window))->keyHandlerCallback(
                        window, key, scancode, action, mods);
            };

            auto func3 = [](GLFWwindow* window, int width, int height)
            {
                static_cast<Game*>
                    (glfwGetWindowUserPointer(window))->windowResizedCallback(
                        window, width, height);
            };

            auto func4 = [](GLFWwindow* window, unsigned int char_pressed)
            {
                static_cast<Game*>
                    (glfwGetWindowUserPointer(window))->charCallback(window,
                        char_pressed);
            };

            auto func5 = [](GLFWwindow* window, double x, double y)
            {
                static_cast<Game*>
                    (glfwGetWindowUserPointer(window))->cursorPosCallback(
                        window, x, y);
            };

            glfwSetMouseButtonCallback(window, func);
            glfwSetKeyCallback(window, func2);
            glfwSetWindowSizeCallback(window, func3);
            glfwSetCharCallback(window, func4);
            glfwSetCursorPosCallback(window, func5);
            //glfwSetScrollCallback(window, mouseWheelCallback);

            // Dark blue background
            glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        }
        catch (std::exception &e)
        {
            throw e;
        }
    }

    void
    Game::initSound()
    {
        AudioManager::getInstance()->loadStream(SOUNDS_PATH + MENU_MUSIC);
        AudioManager::getInstance()->loadStream(SOUNDS_PATH + IN_GAME_MUSIC);
        AudioManager::getInstance()->loadSound(SOUNDS_PATH + SAY_GAME_OVER);
        AudioManager::getInstance()->loadSound(SOUNDS_PATH + GAME_OVER_TUNE);
        AudioManager::getInstance()->loadStream(SOUNDS_PATH + VICTORY_MUSIC);
        AudioManager::getInstance()->playStream(SOUNDS_PATH + MENU_MUSIC, 
            ui->getMusicVolumeFloat());

        gameOverTuneDuration = AudioManager::getInstance()->getSoundLengthMS(
            SOUNDS_PATH + GAME_OVER_TUNE);

        inGameMusicStarted = false;
        musicFadeDuration = 2000;
        gameOverTunePlayed = false;
        gameOverSaid = false;
        victoryMusicStarted = false;
    }
}
