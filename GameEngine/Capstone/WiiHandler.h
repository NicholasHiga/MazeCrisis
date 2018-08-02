#pragma once
#include "wiiuse.h"
#include <memory>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "WiiCursorSmoother2.h"
#include <Eigen/Dense>

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

        int getBottomScreenReloadPercentage() const;
        void setBottomScreenReloadPercentage(int percent); // Percent should be
                                                           // from 0 - 100, as
                                                           // opposed to 0 - 1
        int getReloadingHeightBoundary();

        void update(float deltaTime);
        void wiiEventCallback(struct wiimote_t* wm);
        void windowResizedCallback(int width, int height);
        //void cursorPosCallback(GLFWwindow* window, double x, double y);

    private:
        void calibrateController();
        void getUniqueWiiSensorInputs(ivec2 updatedPoint);

        // Wii Related
        //GLFWwindow *window;
        //GLushort windowWidth, windowHeight;
        Game *game;
        wiimote** wiimotes = NULL;
        const int NUM_PLAYERS = 1;
        bool wiiControllerConnected = false;

        static const int NUM_CALIBRATION_POINTS = 4;

        // ONLY handles 1 controller calibration.
        bool firstCalibrationTargetSet, controllerCalibrated;
        int currentCalibrationTargetNum = 0;
        Eigen::Vector2f calibrationPoints[NUM_CALIBRATION_POINTS];
        Eigen::Vector2f calibrationTargetPositions[NUM_CALIBRATION_POINTS];
        Eigen::Vector2f calibrationAlphas, calibrationBetas, calibrationDeltas;

        // Determines what percentage of the bottom screen the
        // cursor needs to be in to reload with the Wii.
        int bottomScreenReloadPercentage;
        int reloadingHeightBoundary;

        static const int NUM_WII_POINTS = 3;
        std::unique_ptr<WiiCursorSmoother2> cursorSmoother;
        vec2 smoothedCursorPosition;
        int numFramesNoWiiSensorUpdate = 0;
        bool wiiSensorUpdated = false;
        ivec2 lastUniqueWiiSensorInputs[NUM_WII_POINTS];
        vec2 targetCursorPos;
        bool cursorSmootherInitialized = false;
    };
}