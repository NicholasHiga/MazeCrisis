#pragma once
#define _USE_MATH_DEFINES

#include <glm/glm.hpp>
#include <sstream>

class Mat4x4;

class Quaternion
{
public:
    Quaternion();
    Quaternion(const Quaternion& copy);
    Quaternion(float w, float x, float y, float z);
    Quaternion(float angle, glm::vec3 axis, bool isRadians = true);
    Quaternion(float yaw, float pitch, float roll, bool isRadians = true);

    float X() const;
    float Y() const;
    float Z() const;
    float W() const;

    void X(float val);
    void Y(float val);
    void Z(float val);
    void W(float val);

    ///-------------------STATIC METHODS START-----------------------\\\

    /// converToMatrix4()
    /// Convert the given quaternion to a 4x4 matrix.
    ///
    /// @param[in]      q
    ///                 Quaternion to convert.
    ///
    static Mat4x4 convertToMatrix4(Quaternion q);

    /// conjugate()
    /// Return the conjugate quaternion of the given quaternion.
    ///
    /// @param[in]      q
    ///                 Quaternion to convert.
    ///
    static Quaternion conjugate(Quaternion q);

    /// inverse()
    /// Get the inverted version of the given quaternion.
    ///
    /// @param[in]      q
    ///                 Quaternion to get inverse..
    ///
    static Quaternion inverse(Quaternion q);
    
    /// normalized()
    /// Get the normalized quaternion of the given q.
    ///
    /// @param[in]      q
    ///                 Quaternion to normalize.
    ///
    static Quaternion normalized(Quaternion q);

    /// magnitude()
    /// Get the magnitude of the given quaternion.
    ///
    /// @param[in]      q
    ///                 Quaternion to calculate magnitude of.
    ///
    static float magnitude(Quaternion q);

    ///-------------------STATIC METHODS END--------------------------\\\

    /// convertToMatrix4()
    /// Return a 4x4 matrix of version of the current quaternion.
    ///
    Mat4x4 convertToMatrix4() const;
    
    /// getConjugate()
    /// Get the conjugate of the current quaternion.
    ///
    Quaternion getConjugate() const;

    /// toConjugate()
    /// Convert the current quaternion in place to its conjugate.
    ///
    void toConjugate();

    /// inverse()
    /// Invert the quaternion in place.
    ///
    void inverse();

    /// normalize()
    /// Normalize the quaternion in place.
    ///
    void normalize();

    /// magnitude()
    /// Get the magnitude of the current quaternion.
    ///
    float magnitude() const;
    
    /// rotationAndVectorToQuatDegrees()
    /// Get a quaternion from a given angle and axis.
    ///
    /// @param[in]      degrees
    ///                 angle in degrees.
    ///
    /// @param[in]      axis
    ///                 Vector axis where the rotation is around.
    ///
    static Quaternion rotationAndVectorToQuatDegrees(float degrees,
        glm::vec3 axis);

    /// rotationAndVectorToQuatRadians()
    /// Get a quaternion from a given angle and axis.
    ///
    /// @param[in]      radians
    ///                 angle in radians.
    ///
    /// @param[in]      axis
    ///                 Vector axis where the rotation is around.
    ///
    static Quaternion rotationAndVectorToQuatRadians(float radians, 
        glm::vec3 axis);

    /// toRotationAndVectorDegrees()
    /// From the current quaternion, calculate the angle and rotation.
    ///
    /// @param[out]     degrees
    ///                 Angle in degrees.
    ///
    /// @param[out]     axis
    ///                 Vector axis.
    ///
    void toRotationAndVectorDegrees(float &degrees, glm::vec3 &axis) const;

    /// toRotationAndVectorRadians()
    /// From the current quaternion, calculate the angle and rotation.
    ///
    /// @param[out]     radians
    ///                 Angle in radians.
    ///
    /// @param[out]     axis
    ///                 Vector axis.
    ///
    void toRotationAndVectorRadians(float &radians, glm::vec3 &axis) const;

    /// eulerToQuatRadians()
    /// Coversion from euler angles to a quaternion, using yaw (z)
    /// pitch (y), roll (x) as the order of applied angles. All
    /// angles in radians.
    ///
    /// @param[in]      yaw
    ///                 z angle.
    ///
    /// @param[in]      pitch
    ///                 y angle.
    ///
    /// @param[in]      roll
    ///                 x angle.
    ///
    static Quaternion eulerToQuatRadians(float yaw, float pitch, float roll);

    /// eulerToQuatRadians()
    /// Coversion from euler angles to a quaternion, using yaw (z)
    /// pitch (y), roll (x) as the order of applied angles. All
    /// angles in degrees.
    ///
    /// @param[in]      yaw
    ///                 z angle.
    ///
    /// @param[in]      pitch
    ///                 y angle.
    ///
    /// @param[in]      roll
    ///                 x angle.
    ///
    static Quaternion eulerToQuatDegrees(float yaw, float pitch, float roll);

    /// quatToEuler()
    /// Conversion from quaternion to euler, applied in yaw (z),
    /// pitch (y), roll (x) order. All angles in radians.
    ///
    /// Using 
    /// http://www.euclideanspace.com/maths
    /// /geometry/rotations/conversions/quaternionToEuler/ 
    /// as a reference.
    ///
    /// @param[out]     yaw
    ///                 z angle.
    ///
    /// @param[out]     pitch
    ///                 y angle.
    ///
    /// @param[out]     yaw
    ///                 x angle.
    ///
    void quatToEulerRadians(float &yaw, float &pitch, float &roll) const;

    /// quatToEuler()
    /// Conversion from quaternion to euler, applied in yaw (z),
    /// pitch (y), roll (x) order. All angles in degrees.
    ///
    /// Using 
    /// http://www.euclideanspace.com/maths/geometry
    /// /rotations/conversions/quaternionToEuler/ 
    /// as a reference.
    ///
    /// @param[out]     yaw
    ///                 z angle.
    ///
    /// @param[out]     pitch
    ///                 y angle.
    ///
    /// @param[out]     yaw
    ///                 x angle.
    ///
    void quatToEulerDegrees(float &yaw, float &pitch, float &roll) const;

    /// toString()
    /// Return a string in the format [ w ( x, y, z) ]
    ///
    std::string toString();
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& vec);

    bool operator==(const Quaternion& b) const;
    bool operator!=(const Quaternion& b) const;
    Quaternion operator=(const Quaternion& b);
    Quaternion operator*(const Quaternion b) const;
    Quaternion operator/(const float b) const;

private:
    float x, y, z, w;
};