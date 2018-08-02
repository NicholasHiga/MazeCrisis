#pragma once
#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Camera.h"
#include "GameObject.h"
#include "SceneNode.h"

namespace MazeCrisis
{
#define DEG_TO_RAD 0.01745329251
#define RAD_TO_DEG 57.2957795131

    class UserInterface;

    enum ACTION_TYPE
    {
        TRANSLATION,
        ROTATION
    };

    struct ActionPath
    {
        ACTION_TYPE actionType;
        GLfloat amount;    // Amount to translate, or rotation depending
                        // on actionType
        GLuint timeToDestination;

        glm::vec3 destination, start;                        // For translations
        GLfloat destinationAngle, startingAngle;            // For rotations
    };

    class AutomaticCamera : public Camera
    {
    public:
        AutomaticCamera(UserInterface *ui, GLuint windowWidth, 
            GLuint windowHeight);
        void autoMoveStraight(GLuint distance, GLuint timeToDestination);
        void autoRotate(GLint numDegrees, GLuint timeToDestination);
        void setCameraPosition(glm::vec3 &position,
            bool updateImmediately = true);
        BoundingBox* getBoundingBox() const;
        void clearPathing();
        void update(float deltaTime);

        std::string getFootstepsSoundFilePath() const;
        void setFootstepsSoundFilePath(
            const std::string &footstepsSoundFilePath);

        UserInterface* getUserInterface() const;
        void setUserInterface(UserInterface *ui);

        static const std::string FOOTSTEPS_SOUND;
            
    private:
        std::vector<ActionPath> pathing;
        std::shared_ptr<GameObject> go;
        std::shared_ptr<SceneNode> invisBoundingBoxNode; 
            // Only used for collision purposes.
        double timeElapsed = 0;

        std::string footstepsSoundFilePath;
        float timeBetweenFootsteps, footstepsSoundTimeStart;

        UserInterface* ui;
    };
}