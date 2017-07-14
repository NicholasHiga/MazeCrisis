#include "Quaternion.h"
#include "Mat4x4.h"

using glm::vec3;
using std::string;
using std::ostream;
using std::stringstream;

Quaternion::Quaternion() 
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

Quaternion::Quaternion(const Quaternion& copy)
{
	w = copy.W();
	x = copy.X();
	y = copy.Y();
	z = copy.Z();
}

Quaternion::Quaternion(float w, float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::Quaternion(float angle, vec3 axis, bool isRadians)
{
	if (isRadians)
		*this = rotationAndVectorToQuatRadians(angle, axis);
	else
		*this = rotationAndVectorToQuatDegrees(angle, axis);
}

Quaternion::Quaternion(float yaw, float pitch, float roll, bool isRadians)
{
	if (isRadians)
		*this = eulerToQuatRadians(yaw, pitch, roll);
	else
		*this = eulerToQuatDegrees(yaw, pitch, roll);
}

float
Quaternion::X() const
{
	return x;
}

float 
Quaternion::Y() const
{
	return y;
}

float 
Quaternion::Z() const
{
	return z;
}

float 
Quaternion::W() const
{
	return w;
}

void 
Quaternion::X(float val)
{
	x = val;
}

void 
Quaternion::Y(float val)
{
	y = val;
}

void 
Quaternion::Z(float val)
{
	z = val;
}

void 
Quaternion::W(float val)
{
	w = val;
}

Mat4x4 
Quaternion::convertToMatrix4(Quaternion q)
{
	Mat4x4 result;
	Quaternion temp = Quaternion(q.W(), q.X(), q.Y(), q.Z());
	temp.normalize();
	result.matrix[0][0] = 1.0f - 2.0f * temp.y * temp.y - 2.0f * temp.z * temp.z;
	result.matrix[0][1] = 2.0f * temp.x * temp.y - 2.0f * temp.z * temp.w;
	result.matrix[0][2] = 2.0f * temp.x * temp.z + 2.0f * temp.y * temp.w;
	result.matrix[0][3] = 0.0f;
	result.matrix[1][0] = 2.0f * temp.x * temp.y + 2.0f * temp.z * temp.w;
	result.matrix[1][1] = 1.0f - 2.0f * temp.x * temp.x - 2.0f * temp.z * temp.z;
	result.matrix[1][2] = 2.0f * temp.y * temp.z - 2.0f * temp.x * temp.w;
	result.matrix[1][3] = 0.0f;
	result.matrix[2][0] = 2.0f * temp.x * temp.z - 2.0f * temp.y * temp.w;
	result.matrix[2][1] = 2.0f * temp.y * temp.z + 2.0f * temp.x * temp.w;
	result.matrix[2][2] = 1.0f - 2.0f * temp.x * temp.x - 2.0f * temp.y * temp.y;
	result.matrix[2][3] = 0.0f;
	result.matrix[3][0] = 0.0f;
	result.matrix[3][1] = 0.0f;
	result.matrix[3][2] = 0.0f;
	result.matrix[3][3] = 1.0f;
	return result;
}

Quaternion 
Quaternion::conjugate(Quaternion q)
{
	Quaternion result;
	result.w = q.W();
	result.x = -q.X();
	result.y = -q.Y();
	result.z = -q.Z();
	return result;
}

Quaternion 
Quaternion::inverse(Quaternion q)
{
	Quaternion result = Quaternion::conjugate(q);
	float magnitude = Quaternion::magnitude(q) * Quaternion::magnitude(q);
	return result / magnitude;
}

Quaternion 
Quaternion::normalized(Quaternion q)
{
	float magnitude = Quaternion::magnitude(q);
	return Quaternion(q.W() / magnitude, q.X() / magnitude, q.Y() / magnitude, q.Z() / magnitude);
}

float 
Quaternion::magnitude(Quaternion q)
{
	return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

Mat4x4 
Quaternion::convertToMatrix4()
{
	return Quaternion::convertToMatrix4(*this);
}

void
Quaternion::toConjugate()
{
	x = -x;
	y = -y;
	z = -z;
}

Quaternion
Quaternion::getConjugate()
{
	return Quaternion::conjugate(*this);
}

void 
Quaternion::inverse()
{
	*this = Quaternion::inverse(*this);
}

void 
Quaternion::normalize()
{
	*this = Quaternion::normalized(*this);
}

float 
Quaternion::magnitude()
{
	return Quaternion::magnitude(*this);
}

Quaternion
Quaternion::rotationAndVectorToQuatDegrees(float degrees, vec3 axis)
{
	return rotationAndVectorToQuatRadians((float)(degrees * M_PI / 180.0f), axis);
}

Quaternion
Quaternion::rotationAndVectorToQuatRadians(float radians, vec3 axis)
{
	Quaternion result;
	axis = glm::normalize(axis);

	result.w = cos(radians / 2.0f);
	result.x = axis.x * sin(radians / 2.0f);
	result.y = axis.y * sin(radians / 2.0f);
	result.z = axis.z * sin(radians / 2.0f);
	return result;
}

void 
Quaternion::toRotationAndVectorDegrees(float &degrees, vec3 &axis)
{
	/*Quaternion q = Quaternion(w, x, y, z);
	float num, radians;

	q.normalize();
	radians = acos(q.w);
	radians *= 2.0f;
	num = sin(radians / 2.0f);
	degrees = radians * 180.0f / M_PI;

	axis.X(q.x / num);
	axis.Y(q.y / num);
	axis.Z(q.z / num);*/
	toRotationAndVectorRadians(degrees, axis);
	degrees *= (float)(180.0f / M_PI);
}

void
Quaternion::toRotationAndVectorRadians(float &radians, vec3 &axis)
{
	Quaternion q = Quaternion(w, x, y, z);
	float num;

	q.normalize();
	radians = acos(q.w);
	radians *= 2.0f;
	num = sin(radians / 2.0f);

	if (num != 0)
	{
		axis.x = q.x / num;
		axis.y = q.y / num;
		axis.z = q.z / num;
	}
	else
	{
		axis.x = q.x;
		axis.y = q.y;
		axis.z = q.z;
	}
}

Quaternion
Quaternion::eulerToQuatRadians(float yaw, float pitch, float roll)
{
	Quaternion q;
	double c1 = cos(yaw / 2);
	double s1 = sin(yaw / 2);
	double c2 = cos(pitch / 2);
	double s2 = sin(pitch / 2);
	double c3 = cos(roll / 2);
	double s3 = sin(roll / 2);
	double c1c2 = c1 * c2;
	double s1s2 = s1 * s2;
	q.w = (float)(c1c2 * c3 - s1s2 * s3);
	q.x = (float)(c1c2 * s3 + s1s2 * c3);
	q.y = (float)(s1 * c2 * c3 + c1 * s2 * s3);
	q.z = (float)(c1 * s2 * c3 - s1 * c2 * s3);
	return q;
}

Quaternion 
Quaternion::eulerToQuatDegrees(float yaw, float pitch, float roll)
{
	float yawRad, pitchRad, rollRad;
	yawRad = (float)(yaw * M_PI / 180.0f);
	pitchRad = (float)(pitch * M_PI / 180.0f);
	rollRad = (float)(roll * M_PI / 180.0f);
	return eulerToQuatRadians(yawRad, pitchRad, rollRad);
}

void
Quaternion::quatToEulerRadians(float &yaw, float &pitch, float &roll)
{
	double sqw = w * w;
	double sqx = x * x;
	double sqy = y * y;
	double sqz = z * z;
	double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	double test = x * y + z * w;
	if (test > 0.499*unit) { // singularity at north pole
		yaw = (float)(2.0f * atan2(x, w));
		pitch = (float)(M_PI / 2.0f);
		roll = 0;
		return;
	}
	if (test < -0.499*unit) { // singularity at south pole
		yaw = -2.0f * atan2(x, w);
		pitch = (float)(-M_PI / 2.0f);
		roll = 0;
		return;
	}
	yaw = (float)(atan2(2.0f * y * w - 2.0f * x * z, sqx - sqy - sqz + sqw));
	pitch = (float)(asin(2.0f * test / unit));
	roll = (float)(atan2(2.0f * x * w - 2.0f * y * z, -sqx + sqy - sqz + sqw));
}

void
Quaternion::quatToEulerDegrees(float &yaw, float &pitch, float &roll)
{
	quatToEulerRadians(yaw, pitch, roll);
	yaw *= (float)(180.0f / M_PI);
	pitch *= (float)(180.0f / M_PI);
	roll *= (float)(180.0f / M_PI);
}

bool 
Quaternion::operator==(const Quaternion& b) const
{
	return b.w == w && b.x == x && b.y == y && b.z == z;
}

bool 
Quaternion::operator!=(const Quaternion& b) const
{
	return b.w != w || b.x != x || b.y != y || b.z != z;
}

Quaternion 
Quaternion::operator=(const Quaternion& b)
{
	w = b.w;
	x = b.x;
	y = b.y;
	z = b.z;
	return *this;
}

Quaternion 
Quaternion::operator*(const Quaternion b) const
{
	Quaternion result;
	vec3 v1(x, y, z);
	vec3 v2(b.x, b.y, b.z);
	vec3 vComp;

	result.w = w * b.w - glm::dot(v1, v2);
	//vComp = w * v2 + b.w * v1 + v1.cross(v2);
	vComp = w * v2 + b.w * v1 + cross(v1, v2);
	result.x = vComp.x;
	result.y = vComp.y;
	result.z = vComp.z;
	return result;
}

Quaternion
Quaternion::operator/(const float b) const
{
	return Quaternion(this->w / b, this->x / b, this->y / b, this->z / b);
}

string
Quaternion::toString()
{
	stringstream s;
	s << "[ " << w << " ( " << x << ", " << y << ", " << z << " ) ]";
	return s.str();
}

ostream& operator<<(ostream& os, const Quaternion& q)
{
	os << "[ " << q.W() << " ( " << q.X() << ", " << q.Y() << ", " << q.Z() << " ) ]";
	return os;
}