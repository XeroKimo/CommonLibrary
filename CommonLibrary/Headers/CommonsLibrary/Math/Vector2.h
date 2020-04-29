#pragma once
#include "MathConstants.h"

namespace CommonsLibrary
{
    struct Vector2
    {
    public:
        union
        {
            float data[2] = { 0 };
            struct { float x, y; };
            struct { float r, g; };
            struct { float u, v; };
        };

    public:
        constexpr Vector2() = default;

        constexpr Vector2(float _x, float _y) : x(_x), y(_y)
        {
        }

        constexpr Vector2 operator+(const Vector2& other) const
        {
            return Vector2(x + other.x, y + other.y);
        }
        constexpr Vector2 operator-(const Vector2& other) const
        {
            return Vector2(x - other.x, y - other.y);
        }
        constexpr Vector2 operator*(const Vector2& other) const
        {
            return Vector2(x * other.x, y * other.y);
        }
        constexpr Vector2 operator/(const Vector2& other) const
        {
            return Vector2(x / other.x, y / other.y);
        }
        constexpr Vector2 operator*(const float& other) const
        {
            return Vector2(x * other, y * other);
        }
        constexpr Vector2 operator/(const float& other) const
        {
            return Vector2(x / other, y / other);
        }

        constexpr Vector2 operator-()
        {
            return *this * -1;
        }

        constexpr void operator+=(const Vector2& other)
        {
            *this = *this + other;
        }
        constexpr void operator-=(const Vector2& other)
        {
            *this = *this - other;
        }
        constexpr void operator*=(const Vector2& other)
        {
            *this = *this * other;
        }
        constexpr void operator/=(const Vector2& other)
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

        constexpr bool operator==(const Vector2& other) const
        {
            return (x == other.x && y == other.y);
        }

    public:
        constexpr float Dot(const Vector2& other) const
        {
            return (x * other.x) + (y * other.y);
        }
        float Magnitude() const
        {
            return sqrtf(MagnitudeSquared());
        }
        constexpr float MagnitudeSquared() const
        {
            return (x * x) + (y * y);
        }
        void Normalize()
        {
            float magnitude = Magnitude();
            x /= magnitude;
            y /= magnitude;
        }
        void Absolute()
        {
            x = abs(x);
            y = abs(y);
        }
        Vector2 GetNormalized() const
        {
            Vector2 normalized = *this;
            normalized.Normalize();
            return normalized;
        }
    };
}