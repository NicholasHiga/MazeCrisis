#include "Plane.h"

using glm::vec3;

Plane::Plane()
{
	a = 0;
	b = 0;
	c = 0;
	d = 0;
}

Plane::Plane(float _a, float _b, float _c, float _d)
{
	setPlane(_a, _b, _c, _d);
}

Plane::Plane(const vec3 &normal, float _d)
{
	*this = calculatePlane(normal, _d);
}

Plane::Plane(const vec3 &point1, const vec3 &point2, const vec3 &point3)
{
	*this = calculatePlane(point1, point2, point3);
}

Plane::Plane(const vec3 &point, const vec3 &normal)
{
	*this = calculatePlane(point, normal);
}

void
Plane::setPlane(float _a, float _b, float _c, float _d)
{
	a = _a;
	b = _b;
	c = _c;
	d = _d;
}

Plane
Plane::calculatePlane(const vec3 &normal, float _d)
{
	vec3 temp = glm::normalize(normal);
	return Plane(temp.x, temp.y, temp.z, _d);
}

Plane
Plane::calculatePlane(const vec3 &point1, const vec3 &point2, const vec3 &point3)
{
	vec3 p12 = vec3(point2 - point1);
	vec3 p23 = vec3(point3 - point2);
	vec3 cross = glm::cross(p12, p23);
	float newD = cross.x * -point1.x + cross.y * -point1.y + cross.z * -point1.z;
	return Plane(cross.x, cross.y, cross.z, newD);
}

Plane 
Plane::calculatePlane(const vec3 &point, const vec3 &normal)
{
	vec3 temp = glm::normalize(normal);
	return Plane(temp.x, temp.y, temp.z, -glm::dot(point, temp));
}

float
Plane::signedDistance(vec3 &pt)
{
	vec3 N(a, b, c);
	return glm::dot(N, pt) + d;
}