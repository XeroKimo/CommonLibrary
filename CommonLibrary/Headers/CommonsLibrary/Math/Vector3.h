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
        constexpr Vector3() = default;

        constexpr Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
        {
        }

        constexpr Vector3 operator+(const Vector3& other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        constexpr Vector3 operator-(const Vector3& other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        constexpr Vector3 operator*(const Vector3& other) const
        {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }
        constexpr Vector3 operator/(const Vector3& other) const
        {
            return Vector3(x / other.x, y / other.y, z / other.z);
        }
        constexpr Vector3 operator*(const float& other) const
        {
            return Vector3(x * other, y * other, z * other);
        }
        constexpr Vector3 operator/(const float& other) const
        {
            return Vector3(x / other, y / other, z / other);
        }

        constexpr Vector3 operator-() const
        {
            return *this * -1;
        }

        constexpr void operator+=(const Vector3& other)
        {
            *this = *this + other;
        }
        constexpr void operator-=(const Vector3& other)
        {
            *this = *this - other;
        }
        constexpr  void operator*=(const Vector3& other)
        {
            *this = *this * other;
        }
        constexpr void operator/=(const Vector3& other)
        {
            *this = *this / other;
        }
        constexpr void operator*=(const float& other)
        {
            *this = *this * other;
        }
        constexpr void operator/=(const float& other)
        {
            *this = *this / other;
        }

        constexpr bool operator==(const Vector3& other) const
        {
            return (x == other.x && y == other.y && z == other.z);
        }

    public:
        constexpr float Dot(const Vector3& other) const
        {
            return (x * other.x) + (y * other.y) + (z * other.z);
        }
        constexpr Vector3 Cross(const Vector3& other) const
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
        constexpr float MagnitudeSquared() const
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
            x = std::abs(x);
            y = std::abs(y);
            z = std::abs(z);
        }
        Vector3 GetNormalized() const
        {
            Vector3 normalized = *this;
            normalized.Normalize();
            return normalized;
        }

        constexpr static Vector3 Forward()
        {
            if(use_Left_Handed_Matrices)
                return Vector3(0, 0, 1);
            else
                return Vector3(0, 0, -1);
        }
        constexpr static Vector3 Up()
        {
            return Vector3(0, 1, 0);
        }
        constexpr static Vector3 Right()
        {
            return Vector3(1, 0, 0);
        }

        constexpr static Vector3 Backward()
        {
            return -Forward();
        }
        constexpr static Vector3 Down()
        {
            return -Up();
        }
        constexpr static Vector3 Left()
        {
            return -Right();
        }
    };
}
