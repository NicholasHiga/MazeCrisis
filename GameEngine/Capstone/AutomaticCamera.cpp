#include "AutomaticCamera.h"

#include <iostream>
#include "Game.h"
#include "RayQuery.h"
#include "Enemy.h"
#include "ModelManager.h"
#include "Common.h"

namespace MazeCrisis
{
	using glm::vec3;

	AutomaticCamera::AutomaticCamera(GLuint windowWidth, GLuint windowHeight)
		: Camera(windowWidth, windowHeight)
	{
		ModelManager::getInstance()->loadCubeModel("CameraBox",
			TEXTURES_PATH + "bcrate.jpg");
		go = std::make_shared<GameObject>("CameraBox");
		invisBoundingBoxNode = std::make_shared<SceneNode>();
		invisBoundingBoxNode->setScale(vec3(20, 20, 20));
		invisBoundingBoxNode->addRenderable(*go);
		// Just doing generic texture, because we're going to keep the
		// model invisible with the bounding box enabled to detect when
		// an enemy touches the camera.
	}

	void
	AutomaticCamera::autoMoveStraight(GLuint distance, GLuint timeToDestination)
	{
		ActionPath ap;
		ap.actionType = ACTION_TYPE::TRANSLATION;
		ap.amount = distance;
		ap.timeToDestination = timeToDestination;
		pathing.push_back(ap);
	}

	void
	AutomaticCamera::autoRotate(GLint numDegrees, GLuint timeToDestination)
	{
		ActionPath ap;
		ap.actionType = ACTION_TYPE::ROTATION;
		ap.amount = DEG_TO_RAD * numDegrees;
		ap.timeToDestination = timeToDestination;
		pathing.push_back(ap);
	}

	void
	AutomaticCamera::setCameraPosition(glm::vec3 &position, bool updateImmediately)
	{
		Camera::setCameraPosition(position, updateImmediately);
		invisBoundingBoxNode->setPosition(position);
	}

	BoundingBox*
	AutomaticCamera::getBoundingBox()
	{
		std::vector<Renderable*> *renderables = invisBoundingBoxNode.get()->getRenderables();
		// Guaranteed only 1 game object.
		if (GameObject* go = dynamic_cast<GameObject*>((*renderables)[0]))
			return go->getBoundingBoxes()[0];
		else
			return nullptr;
	}

	void
	AutomaticCamera::clearPathing()
	{
		pathing.clear();
	}

	void
	AutomaticCamera::update(float deltaTime)
	{
		// TODO Clean up copy pasted code.
		if (!pathing.empty())
		{
			// Initial calculations before moving.
			if (timeElapsed == 0)
			{
				if (pathing[0].actionType == ACTION_TYPE::TRANSLATION)
				{
					pathing[0].start = getCameraPosition();
					pathing[0].destination = getCameraPosition() +
						glm::normalize(frustum->getCameraDirection())
						* (float)pathing[0].amount;
				}
				else
				{
					vec3 v1, v2;
					v1 = frustum->getCameraDirection();
					v2 = vec3(0, 0, 1);
					pathing[0].startingAngle = glm::atan(v1.z, v1.x) - glm::atan(v2.z, v2.x);

					if (pathing[0].startingAngle < 0)
						pathing[0].startingAngle += glm::two_pi<float>();

					pathing[0].startingAngle = 360 * DEG_TO_RAD - pathing[0].startingAngle;
					pathing[0].destinationAngle = pathing[0].startingAngle + pathing[0].amount;
				}
			}

			// Destination of pathing[0] reached.
			if (timeElapsed >= pathing[0].timeToDestination)
			{
				// Making sure the value ends up at the exact position,
				// instead of really close to the destination.
				if (pathing[0].actionType == ACTION_TYPE::TRANSLATION)
				{
					vec3 dir = glm::normalize(pathing[0].destination - getCameraPosition());
					setCameraPosition(pathing[0].destination);
					dir *= 3;
					setSceneCenter(getSceneCenter() + dir);
					vec3 pos = getCameraPosition();
					std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
				}
				else
				{
					// Maintaining scene center to be 3 in front of the camera position.
					vec3 dir(sin(pathing[0].destinationAngle), 0, cos(pathing[0].destinationAngle));
					dir = glm::normalize(dir);
					dir *= 3;
					setSceneCenter(getCameraPosition() + dir);
				}

				timeElapsed = 0;
				pathing.erase(pathing.begin());
			}
			else
			{
				// Actual movement of the camera
				if (pathing[0].actionType == ACTION_TYPE::TRANSLATION)
				{
					setCameraPosition(glm::mix(pathing[0].start,
						pathing[0].destination,
						timeElapsed / pathing[0].timeToDestination));

					// Maintaining scene center to be 3 in front of the camera position.
					vec3 dir = glm::normalize(pathing[0].destination - getCameraPosition());
					dir *= 3;
					setSceneCenter(getSceneCenter() + dir);
				}
				else
				{
					float angle = glm::mix(pathing[0].startingAngle,
						pathing[0].destinationAngle,
						timeElapsed / pathing[0].timeToDestination);

					// Maintaining scene center to be 3 in front of the camera position.
					vec3 dir(sin(angle), 0, cos(angle));
					dir = glm::normalize(dir);
					dir *= 3;
					setSceneCenter(getCameraPosition() + dir);
				}
				timeElapsed += deltaTime;
			}
		}
	}
}