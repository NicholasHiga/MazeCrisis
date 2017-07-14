#pragma once
#include <glm/glm.hpp>

// Plane has to be in form ax + by + cz + d = 0,
// NOT ax + by + cz = d
class Plane
{
public:
	Plane();
	Plane(float _a, float _b, float _c, float _d);
	Plane(const glm::vec3 &normal, float _d);
	Plane(const glm::vec3 &point1, const glm::vec3 &point2, const glm::vec3 &point3);
	Plane(const glm::vec3 &point, const glm::vec3 &normal);

	void setPlane(float _a, float _b, float _c, float _d);
	Plane calculatePlane(const glm::vec3 &normal, float _d);
	Plane calculatePlane(const glm::vec3 &point1, const glm::vec3 &point2, const glm::vec3 &point3);
	Plane calculatePlane(const glm::vec3 &point, const glm::vec3 &normal);

	// Positive = same side as normal, negative = opposite.
	float signedDistance(glm::vec3 &pt);

private:
	float a, b, c, d;
};
#pragma once
