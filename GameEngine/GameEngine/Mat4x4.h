#pragma once

#define _USE_MATH_DEFINES

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Mat4x4
{
public:
	std::vector<std::vector<float>> matrix;

	Mat4x4();
	Mat4x4(float xx, float xy, float xz, float xw,
		   float yx, float yy, float yz, float yw,
		   float zx, float zy, float zz, float zw,
		   float wx, float wy, float wz, float ww);
	Mat4x4(float matrix[16], bool rowMajor);
	Mat4x4(const Mat4x4& copy);

	float xx() const { return matrix[0][0]; }
	float xy() const { return matrix[0][1]; }
	float xz() const { return matrix[0][2]; }
	float xw() const { return matrix[0][3]; }
	float yx() const { return matrix[1][0]; }
	float yy() const { return matrix[1][1]; }
	float yz() const { return matrix[1][2]; }
	float yw() const { return matrix[1][3]; }
	float zx() const { return matrix[2][0]; }
	float zy() const { return matrix[2][1]; }
	float zz() const { return matrix[2][2]; }
	float zw() const { return matrix[2][3]; }
	float wx() const { return matrix[3][0]; }
	float wy() const { return matrix[3][1]; }
	float wz() const { return matrix[3][2]; }
	float ww() const { return matrix[3][3]; }

	void xx(float val) { matrix[0][0] = val; }
	void xy(float val) { matrix[0][1] = val; }
	void xz(float val) { matrix[0][2] = val; }
	void xw(float val) { matrix[0][3] = val; }
	void yx(float val) { matrix[1][0] = val; }
	void yy(float val) { matrix[1][1] = val; }
	void yz(float val) { matrix[1][2] = val; }
	void yw(float val) { matrix[1][3] = val; }
	void zx(float val) { matrix[2][0] = val; }
	void zy(float val) { matrix[2][1] = val; }
	void zz(float val) { matrix[2][2] = val; }
	void zw(float val) { matrix[2][3] = val; }
	void wx(float val) { matrix[3][0] = val; }
	void wy(float val) { matrix[3][1] = val; }
	void wz(float val) { matrix[3][2] = val; }
	void ww(float val) { matrix[3][3] = val; }
	

	///-------------------STATIC METHODS START-----------------------\\\

	/// zero()
	/// Get a zero matrix.
	///
	static Mat4x4 zero();

	/// identity()
	/// Get an identity matrix.
	///
	static Mat4x4 identity();

	/// determinant()
	/// Get the determinant of the given matrix.
	///
	/// @oaram[in]      mat
	///                 Matrix to calculate determinant of.
	///
	static float determinant(Mat4x4 mat);

	/// transpose()
	/// Tranpose the given matrix.
	///
	/// @param[in]      mat
	///                 Matrix to be transposed.
	///
	static Mat4x4 transpose(const Mat4x4 mat);
	
	/// invert()
	/// Invert the given matrix.
	///
	/// @param[in]      mat
	///                 Matrix to invert.
	///
	/// @return         bool
	///                 If matrix was successfully inverted.
	///
	static bool invert(Mat4x4 *mat);

	/// translate()
	/// Return a translation matrix for the given x, y, z
	/// translations.
	///
	/// @param[in]      x
	///                 x translation.
	///
	/// @param[in]      y
	///                 y translation.
	///
	/// @param[in]      z
	///                 z translation.
	///
	static Mat4x4 translate(float x, float y, float z);

	/// scale(){
	/// Returns a scalar transform matrix of the given x, y, z
	/// scaling.
	///
	/// @param[in]      x
	///                 x scaling.
	///
	/// @param[in]      y
	///                 y scaling.
	/// 
	/// @param[in]      z
	///                 z scaling
	///
	static Mat4x4 scale(float x, float y, float z);

	/// rotationXRadians()
	/// Generate a rotation matrix that rotates the 
	/// given amount of radians around the x axis.
	///
	/// @param[in]      radians
	///                 Number of radians.
	///
	static Mat4x4 rotationXRadians(float radians);

	/// rotationXDegrees()
	/// Generate a rotation matrix that rotates the 
	/// given amount of degrees around the x axis.
	///
	/// @param[in]      degrees
	///                 Number of degrees.
	///
	static Mat4x4 rotationXDegrees(float degrees);

	/// rotationYRadians()
	/// Generate a rotation matrix that rotates the 
	/// given amount of radians around the y axis.
	///
	/// @param[in]      radians
	///                 Number of radians.
	///
	static Mat4x4 rotationYRadians(float radians);

	/// rotationYDegrees()
	/// Generate a rotation matrix that rotates the 
	/// given amount of degrees around the y axis.
	///
	/// @param[in]      degrees
	///                 Number of degrees.
	///
	static Mat4x4 rotationYDegrees(float degrees);

	/// rotationZRadians()
	/// Generate a rotation matrix that rotates the 
	/// given amount of radians around the z axis.
	///
	/// @param[in]      radians
	///                 Number of radians.
	///
	static Mat4x4 rotationZRadians(float radians);

	/// rotationZDegrees()
	/// Generate a rotation matrix that rotates the 
	/// given amount of degrees around the z axis.
	///
	/// @param[in]      degrees
	///                 Number of degrees.
	///
	static Mat4x4 rotationZDegrees(float degrees);

	static Mat4x4 rotationRadians(float radians, glm::vec3 &axis);

	static Mat4x4 rotationDegrees(float degrees, glm::vec3 &axis);

	static Mat4x4 convertFromGLM(glm::mat4 m);

	static std::vector<GLfloat> getOpenGLValues(Mat4x4 m);

	///-------------------STATIC METHODS END-----------------------\\\

	/// setZero()
	/// Set the current matrix to zero.
	///
	void setZero();

	/// setIdentity()
	/// Set the current matrix to the identity matrix.
	///
	void setIdentity();

	/// setValues()
	/// Set all the values in the matrix.
	///
	void setValues(float xx, float xy, float xz, float xw,
				   float yx, float yy, float yz, float yw,
				   float zx, float zy, float zz, float zw,
				   float wx, float wy, float wz, float ww);

	/// determinant()
	/// Calculate the determinant of the current matrix.
	///
	float determinant() const;

	/// transpose()
	/// Tranpose the matrix in place.
	///
	void transpose();

	/// invert()
	/// Invert the matrix in place.
	///
	bool invert();

	/// setTranslation()
	/// Set the current matrix to be a translation matrix
	/// translated by the given x, y, z.
	///
	/// @param[in]      x
	///                 x to translate by.
	///
	/// @param[in[      y
	///                 y to translate by.
	///
	/// @param[in]      z
	///                 z to translate by.
	///
	void setTranslation(float x, float y, float z);

	/// scale(){
	/// Sets the current matrix to a scalar transform 
	/// matrix of the given x, y, z scaling.
	///
	/// @param[in]      x
	///                 x scaling.
	///
	/// @param[in]      y
	///                 y scaling.
	/// 
	/// @param[in]      z
	///                 z scaling
	///
	void setScale(float x, float y, float z);

	/// setRotationXRadians()
	/// Set the current matrix to a rotation matrix that rotates the 
	/// given amount of radians around the x axis.
	///
	/// @param[in]      radians
	///                 Number of radians.
	///
	void setRotationXRadians(float radians);

	/// setRotationXDegrees()
	/// Set the current matrix to a rotation matrix that rotates the 
	/// given amount of degrees around the x axis.
	///
	/// @param[in]      degrees
	///                 Number of degrees.
	///
	void setRotationXDegrees(float degrees);

	/// setRotationXYadians()
	/// Set the current matrix to a rotation matrix that rotates the 
	/// given amount of radians around the y axis.
	///
	/// @param[in]      radians
	///                 Number of radians.
	///
	void setRotationYRadians(float radians);

	/// setRotationYDegrees()
	/// Set the current matrix to a rotation matrix that rotates the 
	/// given amount of degrees around the y axis.
	///
	/// @param[in]      degrees
	///                 Number of degrees.
	///
	void setRotationYDegrees(float degrees);

	/// setRotationXZRadians()
	/// Set the current matrix to a rotation matrix that rotates the 
	/// given amount of radians around the z axis.
	///
	/// @param[in]      radians
	///                 Number of radians.
	///
	void setRotationZRadians(float radians);

	/// setRotationZDegrees()
	/// Set the current matrix to a rotation matrix that rotates the 
	/// given amount of degrees around the z axis.
	///
	/// @param[in]      degrees
	///                 Number of degrees.
	///
	void setRotationZDegrees(float degrees);

	bool operator==(const Mat4x4& b) const;
	bool operator!=(const Mat4x4& b) const;
	Mat4x4 operator=(const Mat4x4& b);
	Mat4x4 operator*(const Mat4x4& b);
	glm::vec3 operator*(const glm::vec3& b);

	/// printMatrix()
	/// Print the current matrix.
	///
	void printMatrix() const;
};