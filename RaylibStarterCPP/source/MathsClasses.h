#pragma once
#include <math.h>

/// <summary>
/// MathsClasses includes the Vector3 class created for the Maths for Games assignment, and is used
/// for all position and vector based movement in the game.
/// </summary>
namespace MathsClasses
{
    class Vec3
    {
    public:
        float x, y, z;
        Vec3(float x, float y, float z);

        // Operator overloads
        Vec3 operator +(const Vec3& other);
        Vec3 operator -(const Vec3& other);
        Vec3 operator *(const float& scale);
        Vec3 operator /(const float& rhs);

        float MagnitudeSqr();
        float Magnitude();
        float Distance(Vec3 other);
        float DistanceSqr(Vec3 other);
        void Normalise();
        Vec3 GetNormalised();
        float Dot(Vec3 other);
        Vec3 Cross(Vec3 other);
        float AngleBetween(Vec3 other);

    private:
        static Vec3 Min(Vec3 a, Vec3 b);
        static Vec3 Max(Vec3 a, Vec3 b);

        Vec3 Clamp(Vec3 inputValue, Vec3 minimumValue, Vec3 maximumValue);
    };
};