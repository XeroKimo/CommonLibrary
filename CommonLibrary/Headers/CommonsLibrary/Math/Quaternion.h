#pragma once
#include "Vector4.h"
#include "Matrix4x4.h"

namespace CommonsLibrary
{
    struct Quaternion
    {
        union
        {
            //float data[4] = { 0 };
            struct { float x, y, z, w; };
            struct { float i, j, k, r; };
        }; 

    public:
        constexpr Quaternion() : x(0), y(0), z(0), w(1) {}
        constexpr Quaternion(float _w, float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(_w) {}
        constexpr void Identity() { w = 1; x = 0; y = 0; z = 0; }

        void Rotate(Vector3 axis, float angle)
        {
            angle = angle / 180 * static_cast<float>(pi) / 2;
            Quaternion temp;
            float sinAngle = sin(angle);
            temp.w = cos(angle);
            temp.x = axis.x * sinAngle;
            temp.y = axis.y * sinAngle;
            temp.z = axis.z * sinAngle;

            *this = *this * temp;
        }

        constexpr Quaternion operator+ (const Quaternion& other) const
        {
            Quaternion result;
            result.w = w + other.w;
            result.x = x + other.x;
            result.y = y + other.y;
            result.z = z + other.z;

            return result;
        }
        constexpr Quaternion operator- (const Quaternion& other) const
        {
            Quaternion result;
            result.w = w - other.w;
            result.x = x - other.x;
            result.y = y - other.y;
            result.z = z - other.z;

            return result;
        }
        constexpr Quaternion operator* (const Quaternion& other) const
        {
            Quaternion result;
            Matrix4x4 mat
            (
                Vector4(w, -x, -y, -z),
                Vector4(x, w, -z, y),
                Vector4(y, z, w, -x),
                Vector4(z, -y, x, w)
            );
            Vector4 vec(other.w, other.x, other.y, other.z);
            Vector4 res = mat * vec;

            result = Quaternion(res.x, res.y, res.z, res.w);

            return result;
        }
        constexpr Quaternion operator/ (const Quaternion& other) const
        {
            Quaternion result;
            float dividor = other.w * other.w + other.x * other.x + other.y * other.y + other.z * other.z;

            result.w = (other.w * w + other.x * x + other.y * y + other.z * z) / dividor;
            result.x = (other.w * x - other.x * w - other.y * z + other.z * y) / dividor;
            result.y = (other.w * y + other.x * z - other.y * w - other.z * x) / dividor;
            result.z = (other.w * z - other.x * y + other.y * x - other.z * w) / dividor;

            return result;
        }

        constexpr void operator+= (const Quaternion& other)
        {
            *this = *this + other;
        }
        constexpr void operator-= (const Quaternion& other)
        {
            *this = *this - other;
        }
        constexpr void operator*= (const Quaternion& other)
        {
            *this = *this * other;
        }
        constexpr void operator/= (const Quaternion& other)
        {
            *this = *this / other;
        }

        void Normalize()
        {
            Vector4 normalized(x, y, z, w);
            normalized.Normalize();
            w = normalized.w;
            x = normalized.x;
            y = normalized.y;
            z = normalized.z;
        }

        Matrix4x4 Matrix() const
        {
            //https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
            //https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Quaternion-derived_rotation_matrix
            Matrix4x4 matrix;
            matrix.vx.data[0] = 1 - 2 * (y * y + z * z);
            matrix.vx.data[1] = 2 * (x * y - w * z);
            matrix.vx.data[2] = 2 * (w * y + x * z);

            matrix.vy.data[0] = 2 * (w * z + x * y);
            matrix.vy.data[1] = 1 - 2 * (x * x + z * z);
            matrix.vy.data[2] = 2 * (y * z - w * x);

            matrix.vz.data[0] = 2 * (z * x - w * y);
            matrix.vz.data[1] = 2 * (w * x + y * z);
            matrix.vz.data[2] = 1 - 2 * (x * x + y * y);

            return matrix;
        }
    };
}