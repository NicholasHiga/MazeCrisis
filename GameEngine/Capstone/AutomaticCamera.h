#pragma once
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

	enum ACTION_TYPE
	{
		TRANSLATION,
		ROTATION
	};

	struct ActionPath
	{
		ACTION_TYPE actionType;
		GLfloat amount;	// Amount to translate, or rotation depending
						// on actionType
		GLuint timeToDestination;

		glm::vec3 destination, start;						// For translations
		GLfloat destinationAngle, startingAngle;			// For rotations
	};

	[event_source(native)]
	class AutomaticCamera : public Camera
	{
	public:
		AutomaticCamera(GLuint windowWidth, GLuint windowHeight);
		void autoMoveStraight(GLuint distance, GLuint timeToDestination);
		void autoRotate(GLint numDegrees, GLuint timeToDestination);
		void setCameraPosition(glm::vec3 &position,
			bool updateImmediately = true);
		BoundingBox* getBoundingBox() const;
		void clearPathing();
		void update(float deltaTime);
		__event void destinationReached();
			
	private:
		std::vector<ActionPath> pathing;
		std::shared_ptr<GameObject> go;
		std::shared_ptr<SceneNode> invisBoundingBoxNode; 
			// Only used for collision purposes.
		double timeElapsed = 0;
	};
}