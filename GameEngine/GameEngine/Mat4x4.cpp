#include "Mat4x4.h"

using glm::vec3;
using glm::mat4;
using std::vector;

Mat4x4::Mat4x4()
{
	setIdentity();
}

Mat4x4::Mat4x4(float xx, float xy, float xz, float xw,
			   float yx, float yy, float yz, float yw,
			   float zx, float zy, float zz, float zw,
			   float wx, float wy, float wz, float ww)
{
	setValues(xx, xy, xz, xw,
		yx, yy, yz, yw,
		zx, zy, zz, zw,
		wx, wy, wz, ww);
}

Mat4x4::Mat4x4(float matrix[16], bool rowMajor)
{
	if (rowMajor)
	{
		setValues(matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15]);
	}
	else
	{
		setValues(matrix[0], matrix[4], matrix[8], matrix[12],
			matrix[1], matrix[5], matrix[9], matrix[13],
			matrix[2], matrix[6], matrix[10], matrix[14],
			matrix[3], matrix[7], matrix[11], matrix[15]);
	}
}

Mat4x4 
Mat4x4::zero()
{
	Mat4x4 m;
	m.setZero();
	return m;
}

Mat4x4 
Mat4x4::identity()
{
	return Mat4x4();
}

float 
Mat4x4::determinant(Mat4x4 mat)
{
	// Using http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
	// as a reference.

	return mat.xx() * mat.yy() * mat.zz() * mat.ww() +
		mat.xx() * mat.yz() * mat.zw() * mat.wy() +
		mat.xx() * mat.yw() * mat.zy() * mat.wz() +

		mat.xy() * mat.yx() * mat.zw() * mat.wz() +
		mat.xy() * mat.yz() * mat.zx() * mat.ww() +
		mat.xy() * mat.yw() * mat.zz() * mat.wx() +

		mat.xz() * mat.yx() * mat.zy() * mat.ww() +
		mat.xz() * mat.yy() * mat.zw() * mat.wx() +
		mat.xz() * mat.yw() * mat.zx() * mat.wy() +

		mat.xw() * mat.yx() * mat.zz() * mat.wy() +
		mat.xw() * mat.yy() * mat.zx() * mat.wz() +
		mat.xw() * mat.yz() * mat.zy() * mat.wx() -

		mat.xx() * mat.yy() * mat.zw() * mat.wz() -
		mat.xx() * mat.yz() * mat.zy() * mat.ww() -
		mat.xx() * mat.yw() * mat.zz() * mat.wy() -

		mat.xy() * mat.yx() * mat.zz() * mat.ww() -
		mat.xy() * mat.yz() * mat.zw() * mat.wx() -
		mat.xy() * mat.yw() * mat.zx() * mat.wz() -

		mat.xz() * mat.yx() * mat.zw() * mat.wy() -
		mat.xz() * mat.yy() * mat.zx() * mat.ww() -
		mat.xz() * mat.yw() * mat.zy() * mat.wx() -

		mat.xw() * mat.yx() * mat.zy() * mat.wz() -
		mat.xw() * mat.yy() * mat.zz() * mat.wx() -
		mat.xw() * mat.yz() * mat.zx() * mat.wy();
}

Mat4x4
Mat4x4::transpose(const Mat4x4 mat)
{
	Mat4x4 m;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m.matrix[i][j] = mat.matrix[j][i];
	return m;
}

bool
Mat4x4::invert(Mat4x4 *mat)
{
	// Using http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
	// as a reference.

	if (Mat4x4::determinant(*mat) == 0)
		return false;

	Mat4x4 inverse;
	inverse.matrix[0][0] = mat->yy() * mat->zz() * mat->ww() +
		mat->yz() * mat->zw() * mat->wy() +
		mat->yw() * mat->zy() * mat->wz() -
		mat->yy() * mat->zw() * mat->wz() -
		mat->yz() * mat->zy() * mat->ww() -
		mat->yw() * mat->zz() * mat->wy();
	inverse.matrix[0][1] = mat->xy() * mat->zw() * mat->wz() +
		mat->xz() * mat->zy() * mat->ww() +
		mat->xw() * mat->zz() * mat->wy() -
		mat->xy() * mat->zz() * mat->ww() -
		mat->xz() * mat->zw() * mat->wy() -
		mat->xw() * mat->zy() * mat->wz();
	inverse.matrix[0][2] = mat->xy() * mat->yz() * mat->ww() +
		mat->xz() * mat->yw() * mat->wy() +
		mat->xw() * mat->yy() * mat->wz() -
		mat->xy() * mat->yw() * mat->wz() -
		mat->xz() * mat->yy() * mat->ww() -
		mat->xw() * mat->yz() * mat->wy();
	inverse.matrix[0][3] = mat->xy() * mat->yw() * mat->zz() +
		mat->xz() * mat->yy() * mat->zw() +
		mat->xw() * mat->yz() * mat->zy() -
		mat->xy() * mat->yz() * mat->zw() -
		mat->xz() * mat->yw() * mat->zy() -
		mat->xw() * mat->yy() * mat->zz();
	inverse.matrix[1][0] = mat->yx() * mat->zw() * mat->wz() +
		mat->yz() * mat->zx() * mat->ww() +
		mat->yw() * mat->zz() * mat->wx() -
		mat->yx() * mat->zz() * mat->ww() -
		mat->yz() * mat->zw() * mat->wx() -
		mat->yw() * mat->zx() * mat->wz();
	inverse.matrix[1][1] = mat->xx() * mat->zz() * mat->ww() +
		mat->xz() * mat->zw() * mat->wx() +
		mat->xw() * mat->zx() * mat->wz() -
		mat->xx() * mat->zw() * mat->wz() -
		mat->xz() * mat->zx() * mat->ww() -
		mat->xw() * mat->zz() * mat->wx();
	inverse.matrix[1][2] = mat->xx() * mat->yw() * mat->wz() +
		mat->xz() * mat->yx() * mat->ww() +
		mat->xw() * mat->yz() * mat->wx() -
		mat->xx() * mat->yz() * mat->ww() -
		mat->xz() * mat->yw() * mat->wx() -
		mat->xw() * mat->yx() * mat->wz();
	inverse.matrix[1][3] = mat->xx() * mat->yz() * mat->zw() +
		mat->xz() * mat->yw() * mat->zx() +
		mat->xw() * mat->yx() * mat->zz() -
		mat->xx() * mat->yw() * mat->zz() -
		mat->xz() * mat->yx() * mat->zw() -
		mat->xw() * mat->yz() * mat->zx();
	inverse.matrix[2][0] = mat->yx() * mat->zy() * mat->ww() +
		mat->yy() * mat->zw() * mat->wx() +
		mat->yw() * mat->zx() * mat->wy() -
		mat->yx() * mat->zw() * mat->wy() -
		mat->yy() * mat->zx() * mat->ww() -
		mat->yw() * mat->zy() * mat->wx();
	inverse.matrix[2][1] = mat->xx() * mat->zw() * mat->wy() +
		mat->xy() * mat->zx() * mat->ww() +
		mat->xw() * mat->zy() * mat->wx() -
		mat->xx() * mat->zy() * mat->ww() -
		mat->xy() * mat->zw() * mat->wx() -
		mat->xw() * mat->zx() * mat->wy();
	inverse.matrix[2][2] = mat->xx() * mat->yy() * mat->ww() +
		mat->xy() * mat->yw() * mat->wx() +
		mat->xw() * mat->yx() * mat->wy() -
		mat->xx() * mat->yw() * mat->wy() -
		mat->xy() * mat->yx() * mat->ww() -
		mat->xw() * mat->yy() * mat->wx();
	inverse.matrix[2][3] = mat->xx() * mat->yw() * mat->zy() +
		mat->xy() * mat->yx() * mat->zw() +
		mat->xw() * mat->yy() * mat->zx() -
		mat->xx() * mat->yy() * mat->zw() -
		mat->xy() * mat->yw() * mat->zx() -
		mat->xw() * mat->yx() * mat->zy();
	inverse.matrix[3][0] = mat->yx() * mat->zz() * mat->wy() +
		mat->yy() * mat->zx() * mat->wz() +
		mat->yz() * mat->zy() * mat->wx() -
		mat->yx() * mat->zy() * mat->wz() -
		mat->yy() * mat->zz() * mat->wx() -
		mat->yz() * mat->zx() * mat->wy();
	inverse.matrix[3][1] = mat->xx() * mat->zy() * mat->wz() +
		mat->xy() * mat->zz() * mat->wx() +
		mat->xz() * mat->zx() * mat->wy() -
		mat->xx() * mat->zz() * mat->wy() -
		mat->xy() * mat->zx() * mat->wz() -
		mat->xz() * mat->zy() * mat->wx();
	inverse.matrix[3][2] = mat->xx() * mat->yz() * mat->wy() +
		mat->xy() * mat->yx() * mat->wz() +
		mat->xz() * mat->yy() * mat->wx() -
		mat->xx() * mat->yy() * mat->wz() -
		mat->xy() * mat->yz() * mat->wx() -
		mat->xz() * mat->yx() * mat->wy();
	inverse.matrix[3][3] = mat->xx() * mat->yy() * mat->zz() +
		mat->xy() * mat->yz() * mat->zx() +
		mat->xz() * mat->yx() * mat->zy() -
		mat->xx() * mat->yz() * mat->zy() -
		mat->xy() * mat->yx() * mat->zz() -
		mat->xz() * mat->yy() * mat->zx();
	*mat = inverse;
	return true;
}

Mat4x4 
Mat4x4::translate(float x, float y, float z)
{
	Mat4x4 m;
	m.matrix[0][3] = x;
	m.matrix[1][3] = y;
	m.matrix[2][3] = z;
	return m;
}

Mat4x4 
Mat4x4::scale(float x, float y, float z)
{
	Mat4x4 m;
	m.matrix[0][0] = x;
	m.matrix[1][1] = y;
	m.matrix[2][2] = z;
	return m;
}

Mat4x4
Mat4x4::rotationXRadians(float radians)
{
	Mat4x4 m;
	m.matrix[1][1] = cosf(radians);
	m.matrix[1][2] = -sinf(radians);
	m.matrix[2][1] = sinf(radians);
	m.matrix[2][2] = cosf(radians);
	return m;
}

Mat4x4 
Mat4x4::rotationXDegrees(float degrees)
{
	Mat4x4 m;
	float radians = degrees * M_PI / 180.0f;
	m.matrix[1][1] = cosf(radians);
	m.matrix[1][2] = -sinf(radians);
	m.matrix[2][1] = sinf(radians);
	m.matrix[2][2] = cosf(radians);
	return m;
}

Mat4x4 
Mat4x4::rotationYRadians(float radians)
{
	Mat4x4 m;
	m.matrix[0][0] = cosf(radians);
	m.matrix[0][2] = sinf(radians);
	m.matrix[2][0] = -sinf(radians);
	m.matrix[2][2] = cosf(radians);
	return m;
}

Mat4x4 
Mat4x4::rotationYDegrees(float degrees)
{
	Mat4x4 m;
	float radians = degrees * M_PI / 180.0f;
	m.matrix[0][0] = cosf(radians);
	m.matrix[0][2] = sinf(radians);
	m.matrix[2][0] = -sinf(radians);
	m.matrix[2][2] = cosf(radians);
	return m;
}

Mat4x4 
Mat4x4::rotationZRadians(float radians)
{
	Mat4x4 m;
	m.matrix[0][0] = cosf(radians);
	m.matrix[0][1] = -sinf(radians);
	m.matrix[1][0] = sinf(radians);
	m.matrix[1][1] = cosf(radians);
	return m;
}

Mat4x4 
Mat4x4::rotationZDegrees(float degrees)
{
	Mat4x4 m;
	float radians = degrees * M_PI / 180.0f;
	m.matrix[0][0] = cosf(radians);
	m.matrix[0][1] = -sinf(radians);
	m.matrix[1][0] = sinf(radians);
	m.matrix[1][1] = cosf(radians);
	return m;
}

Mat4x4
Mat4x4::rotationRadians(float radians, vec3 &axis)
{
	Mat4x4 m;
	float c = cosf(radians);
	float s = sinf(radians);
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	m.matrix[0][0] = x * x * (1.0f - c) + c;
	m.matrix[0][1] = x * y * (1.0f - c) - z * s;
	m.matrix[0][2] = x * z * (1.0f - c) + y * s;

	m.matrix[1][0] = y * x * (1.0f - c) + z * s;
	m.matrix[1][1] = y * y * (1.0f - c) + c;
	m.matrix[1][2] = y * z * (1.0f - c) - x * s;

	m.matrix[2][0] = x * z * (1.0f - c) - y * s;
	m.matrix[2][1] = y * z * (1.0f - c) + x * s;
	m.matrix[2][2] = z * z * (1.0f - c) + c;

	return m;
}

Mat4x4
Mat4x4::rotationDegrees(float degrees, vec3 &axis)
{
	float radians = degrees * M_PI / 180.0f;
	return rotationRadians(radians, axis);
}

Mat4x4
Mat4x4::convertFromGLM(mat4 m)
{
	Mat4x4 ret;
	ret.setValues(m[0][0], m[0][1], m[0][2], m[0][3],
		m[1][0], m[1][1], m[1][2], m[1][3],
		m[2][0], m[2][1], m[2][2], m[2][3],
		m[3][0], m[3][1], m[3][2], m[3][3]);
	return ret;
}

vector<GLfloat>  
Mat4x4::getOpenGLValues(Mat4x4 m)
{
	vector<GLfloat> ret;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			ret.push_back(m.matrix[i][j]);

	return ret;
}

Mat4x4::Mat4x4(const Mat4x4& copy)
{
	setIdentity();
	xx(copy.matrix[0][0]);
	xy(copy.matrix[0][1]);
	xz(copy.matrix[0][2]);
	xw(copy.matrix[0][3]);
	yx(copy.matrix[1][0]);
	yy(copy.matrix[1][1]);
	yz(copy.matrix[1][2]);
	yw(copy.matrix[1][3]);
	zx(copy.matrix[2][0]);
	zy(copy.matrix[2][1]);
	zz(copy.matrix[2][2]);
	zw(copy.matrix[2][3]);
	wx(copy.matrix[3][0]);
	wy(copy.matrix[3][1]);
	wz(copy.matrix[3][2]);
	ww(copy.matrix[3][3]);
}

void
Mat4x4::setZero()
{
	matrix.clear();
	for (int i = 0; i < 4; i++)
	{
		vector<float> r;
		for (int j = 0; j < 4; j++)
			r.push_back(0);
		matrix.push_back(r);
	}
}

void
Mat4x4::setIdentity()
{
	setZero();
	matrix[0][0] = 1;
	matrix[1][1] = 1;
	matrix[2][2] = 1;
	matrix[3][3] = 1;
}

void 
Mat4x4::setValues(float xx, float xy, float xz, float xw,
				  float yx, float yy, float yz, float yw,
				  float zx, float zy, float zz, float zw,
				  float wx, float wy, float wz, float ww)
{
	setZero();
	matrix[0][0] = xx;
	matrix[0][1] = xy;
	matrix[0][2] = xz;
	matrix[0][3] = xw;
	matrix[1][0] = yx;
	matrix[1][1] = yy;
	matrix[1][2] = yz;
	matrix[1][3] = yw;
	matrix[2][0] = zx;
	matrix[2][1] = zy;
	matrix[2][2] = zz;
	matrix[2][3] = zw;
	matrix[3][0] = wx;
	matrix[3][1] = wy;
	matrix[3][2] = wz;
	matrix[3][3] = ww;
}

float 
Mat4x4::determinant()
{
	return Mat4x4::determinant(*this);
}

void
Mat4x4::transpose()
{
	*this = Mat4x4::transpose(*this);
}

bool
Mat4x4::invert()
{
	return Mat4x4::invert(this);
}

void 
Mat4x4::setTranslation(float x, float y, float z)
{
	*this = Mat4x4::translate(x, y, z);
}

void 
Mat4x4::setScale(float x, float y, float z)
{
	*this = Mat4x4::scale(x, y, z);
}

void 
Mat4x4::setRotationXRadians(float radians)
{
	*this = Mat4x4::rotationXRadians(radians);
}

void 
Mat4x4::setRotationXDegrees(float degrees)
{
	*this = Mat4x4::rotationXDegrees(degrees);
}

void 
Mat4x4::setRotationYRadians(float radians)
{
	*this = Mat4x4::rotationYRadians(radians);
}

void 
Mat4x4::setRotationYDegrees(float degrees)
{
	*this = Mat4x4::rotationYDegrees(degrees);
}

void 
Mat4x4::setRotationZRadians(float radians)
{
	*this = Mat4x4::rotationZRadians(radians);
}

void 
Mat4x4::setRotationZDegrees(float degrees)
{
	*this = Mat4x4::rotationZDegrees(degrees);
}

bool 
Mat4x4::operator==(const Mat4x4& b) const
{
	return matrix[0][0] == b.matrix[0][0] &&
		matrix[0][1] == b.matrix[0][1] &&
		matrix[0][2] == b.matrix[0][2] &&
		matrix[0][3] == b.matrix[0][3] &&
		matrix[1][0] == b.matrix[1][0] &&
		matrix[1][1] == b.matrix[1][1] &&
		matrix[1][2] == b.matrix[1][2] &&
		matrix[1][3] == b.matrix[1][3] &&
		matrix[2][0] == b.matrix[2][0] &&
		matrix[2][1] == b.matrix[2][1] &&
		matrix[2][2] == b.matrix[2][2] &&
		matrix[2][3] == b.matrix[2][3] &&
		matrix[3][0] == b.matrix[3][0] &&
		matrix[3][1] == b.matrix[3][1] &&
		matrix[3][2] == b.matrix[3][2] &&
		matrix[3][3] == b.matrix[3][3];
}

bool 
Mat4x4::operator!=(const Mat4x4& b) const
{
	return matrix[0][0] != b.matrix[0][0] ||
		matrix[0][1] != b.matrix[0][1] ||
		matrix[0][2] != b.matrix[0][2] ||
		matrix[0][3] != b.matrix[0][3] ||
		matrix[1][0] != b.matrix[1][0] ||
		matrix[1][1] != b.matrix[1][1] ||
		matrix[1][2] != b.matrix[1][2] ||
		matrix[1][3] != b.matrix[1][3] ||
		matrix[2][0] != b.matrix[2][0] ||
		matrix[2][1] != b.matrix[2][1] ||
		matrix[2][2] != b.matrix[2][2] ||
		matrix[2][3] != b.matrix[2][3] ||
		matrix[3][0] != b.matrix[3][0] ||
		matrix[3][1] != b.matrix[3][1] ||
		matrix[3][2] != b.matrix[3][2] ||
		matrix[3][3] != b.matrix[3][3];
}

Mat4x4 
Mat4x4::operator=(const Mat4x4& b)
{
	setValues(b.matrix[0][0], b.matrix[0][1], b.matrix[0][2], b.matrix[0][3],
		b.matrix[1][0], b.matrix[1][1], b.matrix[1][2], b.matrix[1][3],
		b.matrix[2][0], b.matrix[2][1], b.matrix[2][2], b.matrix[2][3],
		b.matrix[3][0], b.matrix[3][1], b.matrix[3][2], b.matrix[3][3]);
	return *this;
}

Mat4x4 
Mat4x4::operator*(const Mat4x4& b)
{
	Mat4x4 m;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m.matrix[i][j] = matrix[i][0] * b.matrix[0][j] + matrix[i][1] * b.matrix[1][j]
				+ matrix[i][2] * b.matrix[2][j] + matrix[i][3] * b.matrix[3][j];
		}
	}
	return m;
}

vec3
Mat4x4::operator*(const vec3& b)
{
	vec3 temp;
	temp.x = matrix[0][0] * b.x + matrix[0][1] * b.y + matrix[0][2] * b.z + matrix[0][3];
	temp.y = matrix[1][0] * b.x + matrix[1][1] * b.y + matrix[1][2] * b.z + matrix[1][3];
	temp.z = matrix[2][0] * b.x + matrix[2][1] * b.y + matrix[2][2] * b.z + matrix[2][3];
	return temp;
}

void
Mat4x4::printMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f\t", matrix[i][j]);
		}
		printf("\n");
	}
}