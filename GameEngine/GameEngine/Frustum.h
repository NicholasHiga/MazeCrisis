#pragma once
#include "Plane.h"

class BoundingBox;

class Frustum
{
public:
	void setWindowSize(float _fov, float _aspectRatio, float _nearPlaneDistance,
		float _farPlaneDistance); // Call whenever window size is changed.
	void setCamera(const glm::vec3 &_pos, const glm::vec3 &_direction,
		const glm::vec3 &_up);
	bool isPointInFrustum(glm::vec3 &point);
	bool isBoxInFrustum(BoundingBox &box);

	glm::vec3 getCameraPosition() const;
	glm::vec3 getCameraLookAt() const;
	glm::vec3 getCameraUp() const;
	glm::vec3 getCameraDirection() const;

	// ****These functions do not update the frustum,
	// updateFrustum should be manually called after 
	// all the camera properties are set, unless updateImmediately = true

	// We may want to set updateImmediately = false when there is game logic
	// that needs is dependant on using the old camera property before
	// updating.
	void setCameraPosition(const glm::vec3 position, 
		bool updateImmediately = true);
	void setCameraUp(const glm::vec3 &up, bool updateImmediately = true);
	void setCameraLookAt(const glm::vec3 &lookAt, 
		bool updateImmediately = true);

	void updateFrustum();

private:
	enum PLANE { LEFT, RIGHT, TOP, BOTTOM, NEARP, FARP };
	enum POINTS 
	{	
		NEAR_TOP_LEFT, NEAR_TOP_RIGHT, 
		NEAR_BOTTOM_LEFT, NEAR_BOTTOM_RIGHT,
		FAR_TOP_LEFT, FAR_TOP_RIGHT,
		FAR_BOTTOM_LEFT, FAR_BOTTOM_RIGHT
	};

	Plane planes[6]; // Each plane representing each side of the frustum.
	glm::vec3 points[8];	
		// Each point respresents the 8 vertices of the frustum.

	float fieldOfView, aspectRatio;
	glm::vec3 cameraPosition, cameraDirection, cameraUp, cameraLookAt;
	float nearDistance, nearHeight, nearWidth, farDistance, farHeight, farWidth;
	glm::vec3 nearCenter, farCenter; // Center and far positions of 
									 // the near and far planes.
	
	glm::vec3 rightVector; // Vector pointing directly to the right of the 
	                       // frustum.

	/*Vec3 nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
	Vec3 farTopLeft, farTopRight, farBottomLeft, farBottomRight;*/
};