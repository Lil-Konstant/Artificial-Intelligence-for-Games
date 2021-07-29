#include "MathsClasses.h"

namespace MathsClasses
{
    Vec3::Vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Returns a Vector3 (point) who's components are the minimum x, y and z of the two input vectors
    Vec3 Vec3::Min(Vec3 a, Vec3 b)
    {
        return Vec3(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z));
    }

    // Returns a Vector3 (point) who's components are the maximum x, y and z of the two input vectors
    Vec3 Vec3::Max(Vec3 a, Vec3 b)
    {
        return Vec3(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z));
    }

    #pragma region Vector Operation Overrides(+, -, scalar / and scalar*)
    // Overloads the + operator for use between vector3's (uses normal vector addition)
    Vec3 Vec3::operator +(const Vec3& other)
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    // Overloads the - operator for use between vector3's (uses normal vector substraction)
    Vec3 Vec3::operator -(const Vec3& other)
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    // Overloads the * operator for scale multiplication (vector on the lhs)
    Vec3 Vec3::operator *(const float& scale)
    {
        return Vec3(x * scale, y * scale, z * scale);
    }
    // Overloads the / operator for scale division (vector on the lhs)
    Vec3 Vec3::operator /(const float& rhs)
    {
        return Vec3(x / rhs, y / rhs, z / rhs);
    }
    #pragma endregion

    // Returns a Vector3 (point) that's value is between the minimum and maximum inclusive
    Vec3 Vec3::Clamp(Vec3 inputValue, Vec3 minimumValue, Vec3 maximumValue)
    {
        return Max(minimumValue, Min(maximumValue, inputValue));
    }

    // Calculates the square of the vectors length as often that's all we need.
    float Vec3::MagnitudeSqr()
    {
        return (x * x) + (y * y) + (z * z);
    }
    // Calculates the length of this vector
    float Vec3::Magnitude()
    {
        return sqrtf(MagnitudeSqr());
    }

    // Returns the distance from this vector to other as a float
    float Vec3::Distance(Vec3 other)
    {
        // Create a separation vector that point from other to this
        Vec3 separationVec = Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
        // Calculate the length of the separation vector and return it
        return separationVec.Magnitude();
    }
    float Vec3::DistanceSqr(Vec3 other)
    {
        // Create a separation vector that point from other to this
        Vec3 separationVec = Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
        // Calculate the squared length of the separation vector and return it
        return separationVec.MagnitudeSqr();
    }

    // Normalises this vector by divding each component my its magnitude
    void Vec3::Normalise()
    {
        // Calculate the magnitude of this vector
        float magnitude = Magnitude();
        // Divide each component by the magnitude
        this->x /= magnitude;
        this->y /= magnitude;
        this->z /= magnitude;
    }

    // Instead returns a new vector which is the normalised version of this vector
    Vec3 Vec3::GetNormalised()
    {
        // Return the normal vector using the overridden / operator
        Vec3 temp = Vec3(this->x, this->y, this->z);
        return temp / temp.Magnitude();
    }

    // Calculates the dot product of this * other for Vector3's and returns the answer as a float
    float Vec3::Dot(Vec3 other)
    {
        return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
    }

    // Calculates the cross product of this x other for Vector3's and returns the answer as a vector
    Vec3 Vec3::Cross(Vec3 other)
    {
        return Vec3
        (
            (this->y * other.z - this->z * other.y),
            (this->z * other.x - this->x * other.z),
            (this->x * other.y - this->y * other.x)
        );
    }

    // Calculates the angle between this vector and other, and returns the answer in radians
    float Vec3::AngleBetween(Vec3 other)
    {
        // Normalise both the vectors to simplify the dot product
        Normalise();
        other.Normalise();
        // Then take the arccos of the dot product, this returns the angle in radians
        return acosf(Dot(other));
    }
}