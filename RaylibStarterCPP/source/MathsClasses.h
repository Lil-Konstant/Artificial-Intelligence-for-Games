#pragma once
#include <math.h>

namespace MathsClasses
{
    class Vec3
    {
    public:
        float x, y, z;
        Vec3(float x, float y, float z);

        //
        Vec3 operator +(const Vec3& other);
        Vec3 operator -(const Vec3& other);
        Vec3 operator *(const float& scale);
        Vec3 operator /(const float& rhs);

        Vec3 Clamp(Vec3 inputValue, Vec3 minimumValue, Vec3 maximumValue);

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
        // Returns a Vector3 (point) who's components are the minimum x, y and z of the two input vectors
        static Vec3 Min(Vec3 a, Vec3 b)
        {
            return Vec3(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z));
        }

        // Returns a Vector3 (point) who's components are the maximum x, y and z of the two input vectors
        static Vec3 Max(Vec3 a, Vec3 b)
        {
            return Vec3(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z));
        }
    };

    class Matrix3
    {
    public:
        float m1, m4, m7,
              m2, m5, m8,
              m3, m6, m9;

        Matrix3();
        Matrix3(float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9);

        Vec3 GetRow(int rowNumber);
        Vec3 GetColumn(int columnNumber);

        // 
        Matrix3 operator +(const Matrix3& other);
        Matrix3 operator -(const Matrix3& other);
        Matrix3 operator *(Matrix3& other);
        Vec3 operator * (const Vec3& rhs);

        void Set(Matrix3 matrix);
        void Set(float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9);

        Matrix3 GetTransposed();

        void SetScaled(float x, float y, float z);
        void Scale(float x, float y, float z);

        void SetRotateX(double radians);
        void RotateX(double radians);

        void SetRotateY(double radians);
        void RotateY(double radians);

        void SetRotateZ(double radians);
        void RotateZ(double radians);

        void RotateEuler(float pitch, float yaw, float roll);

        void SetTranslation(float x, float y);
        void Translate(float x, float y);
    };
};