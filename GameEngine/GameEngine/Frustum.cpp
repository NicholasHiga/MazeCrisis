#include "Frustum.h"
#include "BoundingBox.h"

#define DEG2RAD 3.14159265358979323846/180.0

using glm::vec3;

void 
Frustum::setWindowSize(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance)
{
	fieldOfView = _fieldOfView;
	aspectRatio = _aspectRatio;
	nearDistance = _nearPlaneDistance;
	farDistance = _farPlaneDistance;
	
	float tangent = tanf(_fieldOfView * (float)DEG2RAD * 0.5f);
	nearHeight = 2.0f * tangent * _nearPlaneDistance;
	nearWidth = nearHeight * _aspectRatio;
	farHeight = 2.0f * tangent * _farPlaneDistance;
	farWidth = farHeight * _aspectRatio;
	setCamera(cameraPosition, cameraLookAt, cameraUp);
}

void 
Frustum::setCamera(const vec3 &_pos, const vec3 &_lookAt, const vec3 &_up)
{
	cameraPosition = _pos;
	cameraLookAt = _lookAt;
	cameraDirection = glm::normalize(_lookAt - _pos);
	rightVector = glm::normalize(glm::cross(cameraDirection, _up));
	cameraUp = glm::normalize(glm::cross(rightVector, cameraDirection));

	nearCenter = _pos + (cameraDirection * nearDistance);
	farCenter = _pos + (cameraDirection * farDistance);

	float nearHalfWidth, nearHalfHeight, farHalfWidth, farHalfHeight;
	nearHalfWidth = nearWidth * 0.5f;
	nearHalfHeight = nearHeight * 0.5f;
	farHalfWidth = farWidth * 0.5f;
	farHalfHeight = farHeight * 0.5f;

	// Calculate the position of each point on the frustum.
	points[NEAR_TOP_LEFT] = nearCenter + cameraUp * nearHalfHeight - rightVector * nearHalfWidth;
	points[NEAR_TOP_RIGHT] = nearCenter + cameraUp * nearHalfHeight + rightVector * nearHalfWidth;
	points[NEAR_BOTTOM_LEFT] = nearCenter - cameraUp * nearHalfHeight - rightVector * nearHalfWidth;
	points[NEAR_BOTTOM_RIGHT] = nearCenter - cameraUp * nearHalfHeight + rightVector * nearHalfWidth;
	points[FAR_TOP_LEFT] = farCenter + cameraUp * farHalfHeight - rightVector * farHalfWidth;
	points[FAR_TOP_RIGHT] = farCenter + cameraUp * farHalfHeight + rightVector * farHalfWidth;
	points[FAR_BOTTOM_LEFT] = farCenter - cameraUp * farHalfHeight - rightVector * farHalfWidth;
	points[FAR_BOTTOM_RIGHT] = farCenter - cameraUp * farHalfHeight + rightVector * farHalfWidth;

	// Create each plane using the 3 parameter constructor from the points calculated in the previous set
	// of calculations.
	planes[PLANE::LEFT] = Plane(points[NEAR_TOP_LEFT], points[NEAR_BOTTOM_LEFT], points[FAR_BOTTOM_LEFT]);
	planes[PLANE::RIGHT] = Plane(points[NEAR_BOTTOM_RIGHT], points[NEAR_TOP_RIGHT], points[FAR_BOTTOM_RIGHT]);

	planes[PLANE::TOP] = Plane(points[NEAR_TOP_RIGHT], points[NEAR_TOP_LEFT], points[FAR_TOP_LEFT]);
	planes[PLANE::BOTTOM] = Plane(points[NEAR_BOTTOM_LEFT], points[NEAR_BOTTOM_RIGHT], points[FAR_BOTTOM_RIGHT]);

	planes[PLANE::NEARP] = Plane(points[NEAR_TOP_LEFT], points[NEAR_TOP_RIGHT], points[NEAR_BOTTOM_RIGHT]);
	planes[PLANE::FARP] = Plane(points[FAR_TOP_RIGHT], points[FAR_TOP_LEFT], points[FAR_BOTTOM_LEFT]);
}

bool
Frustum::isPointInFrustum(vec3 &point)
{
	for (int i = 0; i < 6; ++i)
	{
		float temp = planes[i].signedDistance(point);
		if (planes[i].signedDistance(point) < 0)
			return false;
	}

	return true;
}

bool
Frustum::isBoxInFrustum(BoundingBox &box)
{
	// Algorithm from http://iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm

	// TODO: Figure out real reason why objects aren't being rendered when they should be.
	// However, this fix is really effective for little performance loss.
	// Using this buffer because objects that should be rendering are not being rendered
	// without the buffer.
	float buffer = 15.0f;
	vec3 tmpMins = vec3(box.aabbMins.x - buffer, box.aabbMins.y - buffer, box.aabbMins.z - buffer);
	vec3 tmpMaxes = vec3(box.aabbMaxes.x + buffer, box.aabbMaxes.y + buffer, box.aabbMaxes.z + buffer);

	// Initial check to see if each point of the box is on the outside.
	// If all points of the box are on the outside of the frustum,
	// that the box is not inside the frustum.
	for (int i = 0; i < 6; ++i)
	{
		int out = 0;

		out += ((planes[i].signedDistance(vec3(tmpMins.x, tmpMins.y, tmpMins.z)) < 0.0) ? 1 : 0);
		out += ((planes[i].signedDistance(vec3(tmpMaxes.x, tmpMins.y, tmpMins.z)) < 0.0) ? 1 : 0);
		out += ((planes[i].signedDistance(vec3(tmpMins.x, tmpMaxes.y, tmpMins.z)) < 0.0) ? 1 : 0);
		out += ((planes[i].signedDistance(vec3(tmpMaxes.x, tmpMaxes.y, tmpMins.z)) < 0.0) ? 1 : 0);
		out += ((planes[i].signedDistance(vec3(tmpMins.x, tmpMins.y, tmpMaxes.z)) < 0.0) ? 1 : 0);
		out += ((planes[i].signedDistance(vec3(tmpMaxes.x, tmpMins.y, tmpMaxes.z)) < 0.0) ? 1 : 0);
		out += ((planes[i].signedDistance(vec3(tmpMins.x, tmpMaxes.y, tmpMaxes.z)) < 0.0) ? 1 : 0);
		out += ((planes[i].signedDistance(vec3(tmpMaxes.x, tmpMaxes.y, tmpMaxes.z)) < 0.0) ? 1 : 0);

		if (out == 8) 
			return false;
	}

	// Now do the reversed roles, where we check to see if all the points of the frustum are
	// outside the visibility of each of the box vertices. Read the above link for a 
	// better explanation.
	int out;
	out = 0; 
	for (int i = 0; i < 8; ++i)
		out += ((points[i].x > tmpMaxes.x) ? 1 : 0);
	if (out == 8)
		return false;

	out = 0; 
	for (int i = 0; i < 8; ++i)
		out += ((points[i].x < tmpMins.x) ? 1 : 0); 
	if (out == 8)
		return false;

	out = 0;
	for (int i = 0; i < 8; ++i)
		out += ((points[i].y > tmpMaxes.y) ? 1 : 0); 
	if (out == 8)
		return false;
	
	out = 0; 
	for (int i = 0; i < 8; ++i) 
		out += ((points[i].y < tmpMins.y) ? 1 : 0);
	if (out == 8) 
		return false;
	
	out = 0; 
	for (int i = 0; i < 8; ++i)
		out += ((points[i].z > tmpMaxes.z) ? 1 : 0);
	if (out == 8) 
		return false;
	
	out = 0; 
	for (int i = 0; i < 8; ++i)
		out += ((points[i].z < tmpMins.z) ? 1 : 0);
	if (out == 8) 
		return false;

	return true;
}

vec3
Frustum::getCameraPosition() const
{
	return cameraPosition;
}

vec3
Frustum::getCameraLookAt() const
{
	return cameraLookAt;
}

vec3
Frustum::getCameraUp() const
{
	return cameraUp;
}

vec3
Frustum::getCameraDirection() const
{
	return cameraDirection;
}

void 
Frustum::setCameraPosition(const vec3 position, bool updateImmediately)
{
	cameraPosition = position;

	if (updateImmediately)
		updateFrustum();
}

void 
Frustum::setCameraLookAt(const vec3 &lookAt, bool updateImmediately)
{
	cameraLookAt = lookAt;

	if (updateImmediately)
		updateFrustum();
}

void 
Frustum::setCameraUp(const vec3 &up, bool updateImmediately)
{
	cameraUp = up;

	if (updateImmediately)
		updateFrustum();
}

void 
Frustum::updateFrustum()
{
	setCamera(cameraPosition, cameraLookAt, cameraUp);
}