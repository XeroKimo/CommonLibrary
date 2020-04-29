#pragma once
#include "MathConstants.h"
#include "Vector3.h"

namespace CommonsLibrary
{
    struct Vector4
    {
    public:
        union
        {
            float data[4] = { 0 };
            struct { float x, y, z, w; };
            struct { float r, g, b, a; };
        };

    public:
        constexpr Vector4() = default;

        constexpr Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
        {
        }

        constexpr Vector4 operator+(const Vector4& other) const
        {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }
        constexpr Vector4 operator-(const Vector4& other) const
        {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }
        constexpr Vector4 operator*(const Vector4& other) const
        {
            return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
        }
        constexpr Vector4 operator/(const Vector4& other) const
        {
            return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
        }
        constexpr Vector4 operator*(const float& other) const
        {
            return Vector4(x * other, y * other, z * other, w * other);
        }
        constexpr Vector4 operator/(const float& other) const
        {
            return Vector4(x / other, y / other, z / other, w / other);
        }

        constexpr Vector4 operator-()
        {
            return *this * -1;
        }

        constexpr void operator+=(const Vector4& other)
        {
            *this = *this + other;
        }
        constexpr void operator-=(const Vector4& other)
        {
            *this = *this - other;
        }
        constexpr void operator*=(const Vector4& other)
        {
            *this = *this * other;
        }
        constexpr void operator/=(const Vector4& other)
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

        constexpr bool operator==(const Vector4& other) const
        {
            return (x == other.x && y == other.y && z == other.z && w == other.w);
        }

    public:
        constexpr float Dot(const Vector4& other) const
        {
            return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
        }
        float Magnitude() const
        {
            return sqrtf(MagnitudeSquared());
        }
        constexpr float MagnitudeSquared() const
        {
            return (x * x) + (y * y) + (z * z) + (w * w);
        }
        void Normalize()
        {
            float magnitude = Magnitude();
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
            w /= magnitude;
        }
        void Absolute()
        {
            x = abs(x);
            y = abs(y);
            z = abs(z);
            w = abs(w);
        }
        Vector4 GetNormalized() const
        {
            Vector4 normalized = *this;
            normalized.Normalize();
            return normalized;
        }

    };
}