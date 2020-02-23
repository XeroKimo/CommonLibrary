#pragma once
#include "MathConstants.h"
#include "Vector2.h"

namespace CommonsLibrary
{
    struct Vector3
    {
    public:
        union
        {
            float data[3] = { 0 };
            struct { float x, y, z; };
            struct { float r, g, b; };
            struct { float u, v, w; };
        };

    public:
        Vector3() = default;

        Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
        {
        }

        Vector3 operator+(const Vector3& other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        Vector3 operator-(const Vector3& other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        Vector3 operator*(const Vector3& other) const
        {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }
        Vector3 operator/(const Vector3& other) const
        {
            return Vector3(x / other.x, y / other.y, z / other.z);
        }
        Vector3 operator*(const float& other) const
        {
            return Vector3(x * other, y * other, z * other);
        }
        Vector3 operator/(const float& other) const
        {
            return Vector3(x / other, y / other, z / other);
        }

        void operator+=(const Vector3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
        }
        void operator-=(const Vector3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
        }
        void operator*=(const Vector3& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
        }
        void operator/=(const Vector3& other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
        }
        void operator*=(const float& other)
        {
            x *= other;
            y *= other;
            z *= other;
        }
        void operator/=(const float& other)
        {
            x /= other;
            y /= other;
            z /= other;
        }

        bool operator==(const Vector3& other) const
        {
            return (x == other.x && y == other.y && z == other.z);
        }

    public:
        float Dot(const Vector3& other) const
        {
            return (x * other.x) + (y * other.y) + (z * other.z);
        }
        Vector3 Cross(const Vector3& other) const
        {
            Vector3 output;
            output.x = (y * other.z) - (z * other.y);
            output.y = (z * other.x) - (x * other.z);
            output.z = (x * other.y) - (y * other.x);
            return output;
        }
        float Magnitude() const
        {
            return sqrtf(MagnitudeSquared());
        }
        float MagnitudeSquared() const
        {
            return (x * x) + (y * y) + (z * z);
        }
        void Normalize()
        {
            float magnitude = Magnitude();
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
        }
        void Absolute()
        {
            x = abs(x);
            y = abs(y);
            z = abs(z);
        }
        Vector3 GetNormalized() const
        {
            Vector3 normalized = *this;
            normalized.Normalize();
            return normalized;
        }
    };
}