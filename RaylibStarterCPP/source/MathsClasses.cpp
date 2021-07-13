#include "MathsClasses.h"

namespace MathsClasses
{
    Vec3::Vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
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

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    Matrix3::Matrix3()
    {
        m1 = 1; m4 = 0; m7 = 0;
        m2 = 0; m5 = 1; m8 = 0;
        m3 = 0; m6 = 0; m9 = 1;
    }
    // Constructor overload for constructing a 3x3 matrix of floats
    Matrix3::Matrix3(float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9)
    {
        this->m1 = m1; this->m4 = m4; this->m7 = m7;
        this->m2 = m2; this->m5 = m5; this->m8 = m8;
        this->m3 = m3; this->m6 = m6; this->m9 = m9;
    }

    // Returns the input row number as a vector3 (used for matrix dot products)
    Vec3 Matrix3::GetRow(int rowNumber)
    {
        switch (rowNumber)
        {
        case 1:
            return Vec3(m1, m4, m7);
        case 2:
            return Vec3(m2, m5, m8);
        case 3:
            return Vec3(m3, m6, m9);
        }
        return Vec3(0, 0, 0);
    }

    // Returns the input column number as a vector3 (used for matrix dot products)
    Vec3 Matrix3::GetColumn(int columnNumber)
    {
        switch (columnNumber)
        {
        case 1:
            return Vec3(m1, m2, m3);
        case 2:
            return Vec3(m4, m5, m6);
        case 3:
            return Vec3(m7, m8, m9);
        }
        return Vec3(0, 0, 0);
    }

    #pragma region Matrix3 Operator Overloads(+, -, *with matrix3and* with vector3)
    // Overloads the + operator for Matrix3 + Matrix3 calculations, using standard matrix addition
    Matrix3 Matrix3::operator +(const Matrix3& other)
    {
        return Matrix3
        (
            this->m1 + other.m1, this->m2 + other.m2, this->m3 + other.m3,
            this->m4 + other.m4, this->m5 + other.m5, this->m6 + other.m6,
            this->m7 + other.m7, this->m8 + other.m8, this->m9 + other.m9
        );
    }
    // Overloads the - operator for Matrix3 - Matrix3 calculations, using standard matrix subtraction
    Matrix3 Matrix3::operator -(const Matrix3& other)
    {
        return Matrix3
        (
            this->m1 - other.m1, this->m2 - other.m2, this->m3 - other.m3,
            this->m4 - other.m4, this->m5 - other.m5, this->m6 - other.m6,
            this->m7 - other.m7, this->m8 - other.m8, this->m9 - other.m9
        );
    }

    // Overloads the * operator for Matrix3 * Matrix3 calculations, using standard matrix dot product maths
    Matrix3 Matrix3::operator *(Matrix3& other)
    {
        // Get the rows of lhs and the columns of rhs as vectors so they can be dotted together
        Vec3 lhsRow1 = this->GetRow(1);
        Vec3 lhsRow2 = this->GetRow(2);
        Vec3 lhsRow3 = this->GetRow(3);
        Vec3 rhsCol1 = other.GetColumn(1);
        Vec3 rhsCol2 = other.GetColumn(2);
        Vec3 rhsCol3 = other.GetColumn(3);

        return Matrix3
        (
            lhsRow1.Dot(rhsCol1), lhsRow2.Dot(rhsCol1), lhsRow3.Dot(rhsCol1),
            lhsRow1.Dot(rhsCol2), lhsRow2.Dot(rhsCol2), lhsRow3.Dot(rhsCol2),
            lhsRow1.Dot(rhsCol3), lhsRow2.Dot(rhsCol3), lhsRow3.Dot(rhsCol3)
        );
    }

    // Overloads the * operator for Matrix3 * Vector3 calculations, using standard matrix dot products maths (returns a Vector3)
    Vec3 Matrix3::operator * (const Vec3& rhs)
    {
        // Get the rows of lhs as a vector to be dotted with rhs
        Vec3 lhsRow1 = this->GetRow(1);
        Vec3 lhsRow2 = this->GetRow(2);
        Vec3 lhsRow3 = this->GetRow(3);

        return Vec3
        (
            lhsRow1.Dot(rhs),
            lhsRow2.Dot(rhs),
            lhsRow3.Dot(rhs)
        );
    }
    #pragma endregion

    // Sets this matrix to be equal to the input matrix or the input floats
    void Matrix3::Set(Matrix3 matrix)
    {
        this->m1 = matrix.m1; this->m4 = matrix.m4; this->m7 = matrix.m7;
        this->m2 = matrix.m2; this->m5 = matrix.m5; this->m8 = matrix.m8;
        this->m3 = matrix.m3; this->m6 = matrix.m6; this->m9 = matrix.m9;
    }
    void Matrix3::Set(float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9)
    {
        this->m1 = m1; this->m4 = m4; this->m7 = m7;
        this->m2 = m2; this->m5 = m5; this->m8 = m8;
        this->m3 = m3; this->m6 = m6; this->m9 = m9;
    }

    // Flips this matrix around the prime diagonal to find its transpose, and returns this as a new matrix
    Matrix3 Matrix3::GetTransposed()
    {
        return Matrix3
        (
            m1, m4, m7,
            m2, m5, m8,
            m3, m6, m9
        );
    }

    // Turns this matrix into a scale matrix
    void Matrix3::SetScaled(float x, float y, float z)
    {
        m1 = x; m4 = 0; m7 = 0;
        m2 = 0; m5 = y; m8 = 0;
        m3 = 0; m6 = 0; m9 = z;
    }
    // Scales this matrix by the specified amount along the x, y and z axes
    void Matrix3::Scale(float x, float y, float z)
    {
        // Create a scale matrix using the input floats
        Matrix3 scaleMatrix = Matrix3();
        scaleMatrix.SetScaled(x, y, z);
        // Scale this matrix by multiplying it by the scale matrix
        Set((*this) * scaleMatrix);
    }

    // Turns this matrix into a rotation matrix clockwise about the x axis (by amount radians)
    void Matrix3::SetRotateX(double radians)
    {
        Set
        (
            1, 0, 0,
            0, cosf(radians), sinf(radians),
            0, -sinf(radians), cosf(radians)
        );
    }
    // Rotates this matrix by an amount radians about the x axis
    void Matrix3::RotateX(double radians)
    {
        // Create a rotationX matrix using the input radians
        Matrix3 rotationMatrixX = Matrix3();
        rotationMatrixX.SetRotateX(radians);
        // Rotate this matrix by multiplying it by the rotationX matrix
        Set((*this) * rotationMatrixX);
    }

    // Turns this matrix into a rotation matrix clockwise about the y axis (by an amount radians)
    void Matrix3::SetRotateY(double radians)
    {
        Set
        (
            cosf(radians), 0, sinf(radians),
            0, 1, 0,
            sinf(radians), 0, cosf(radians)
        );
    }
    // Rotates this matrix by an amount radians about the y axis
    void Matrix3::RotateY(double radians)
    {
        // Create a rotationY matrix using the input radians
        Matrix3 rotationMatrixY = Matrix3();
        rotationMatrixY.SetRotateY(radians);
        // Rotate this matrix by multiplying it by the rotationY matrix
        Set((*this) * rotationMatrixY);
    }

    // Turns this matrix into a rotation matrix clockwise about the z axis (by an amount radians)
    void Matrix3::SetRotateZ(double radians)
    {
        Set
        (
            cosf(radians), sinf(radians), 0,
            -sinf(radians), cosf(radians), 0,
            0, 0, 1
        );
    }
    // Rotates this matrix by an amount radians about the Z axis
    void Matrix3::RotateZ(double radians)
    {
        // Create a rotationZ matrix using the input radians
        Matrix3 rotationMatrixZ = Matrix3();
        rotationMatrixZ.SetRotateZ(radians);
        // Rotate this matrix by multiplying it by the rotationY matrix
        Set((*this) * rotationMatrixZ);
    }

    // Turns this matrix into a rotation matrix using euler angle rotations about the x, y and z axes respectively
    void Matrix3::RotateEuler(float pitch, float yaw, float roll)
    {
        // Create a matrix for the pitch, yaw and roll rotations, and set them into rotations matrices of the appropriate amount
        Matrix3 RotationMatrixX = Matrix3();
        Matrix3 RotationMatrixY = Matrix3();
        Matrix3 RotationMatrixZ = Matrix3();
        RotationMatrixX.SetRotateX(pitch);
        RotationMatrixY.SetRotateY(yaw);
        RotationMatrixZ.SetRotateZ(roll);
        // Combine the rotation about each axis in this specified order (arbitrary, some engines will differ)
        Set(RotationMatrixZ * RotationMatrixY * RotationMatrixX);
    }

    // Set the translation cells to the input x and y
    void Matrix3::SetTranslation(float x, float y)
    {
        m7 = x;
        m8 = y;
    }
    // Translates this matrix by an amount x along the x, and y along the y
    void Matrix3::Translate(float x, float y)
    {
        // Apply the translation to this matrix
        m7 += x; m8 += y;
    }
}