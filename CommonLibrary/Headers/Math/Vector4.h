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
        Vector4() = default;

        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
        {
        }

        Vector4 operator+(const Vector4& other) const
        {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }
        Vector4 operator-(const Vector4& other) const
        {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }
        Vector4 operator*(const Vector4& other) const
        {
            return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
        }
        Vector4 operator/(const Vector4& other) const
        {
            return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
        }
        Vector4 operator*(const float& other) const
        {
            return Vector4(x * other, y * other, z * other, w * other);
        }
        Vector4 operator/(const float& other) const
        {
            return Vector4(x / other, y / other, z / other, w / other);
        }

        void operator+=(const Vector4& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
        }
        void operator-=(const Vector4& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
        }
        void operator*=(const Vector4& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            w *= other.w;
        }
        void operator/=(const Vector4& other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            w /= other.w;
        }
        void operator*=(const float& other)
        {
            x *= other;
            y *= other;
            z *= other;
            w *= other;
        }
        void operator/=(const float& other)
        {
            x /= other;
            y /= other;
            z /= other;
            w /= other;
        }

        bool operator==(const Vector4& other) const
        {
            return (x == other.x && y == other.y && z == other.z && w == other.w);
        }

    public:
        float Dot(const Vector4& other) const
        {
            return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
        }
        float Magnitude() const
        {
            return sqrtf(MagnitudeSquared());
        }
        float MagnitudeSquared() const
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