#pragma once
#include "MathConstants.h"
#include "Vector4.h"

#ifdef near
#undef near
#endif // near

#ifdef far
#undef far
#endif // far

namespace CommonsLibrary
{
    struct Matrix4x4
    {

    public:
        Vector4 vx;
        Vector4 vy;
        Vector4 vz;
        Vector4 vw;

    public:

        Matrix4x4() { Identity(); }
        Matrix4x4(Vector4 x, Vector4 y, Vector4 z, Vector4 w) { vx = x; vy = y; vz = z; vw = w; }

        Matrix4x4 operator+(const Matrix4x4& other) const
        {
            Matrix4x4 mat;
            mat.vx = vx + other.vx;
            mat.vy = vy + other.vy;
            mat.vz = vz + other.vz;
            mat.vw = vw + other.vw;

            return mat;
        }
        Matrix4x4 operator-(const Matrix4x4& other) const
        {
            Matrix4x4 mat;
            mat.vx = vx - other.vx;
            mat.vy = vy - other.vy;
            mat.vz = vz - other.vz;
            mat.vw = vw - other.vw;

            return mat;
        }
        Matrix4x4 operator*(const Matrix4x4& other) const
        {
            Matrix4x4 mat = other;
            mat.Transpose();
            Matrix4x4 output;

            output.vx.data[0] = vx.Dot(mat.vx);
            output.vx.data[1] = vx.Dot(mat.vy);
            output.vx.data[2] = vx.Dot(mat.vz);
            output.vx.data[3] = vx.Dot(mat.vw);

            output.vy.data[0] = vy.Dot(mat.vx);
            output.vy.data[1] = vy.Dot(mat.vy);
            output.vy.data[2] = vy.Dot(mat.vz);
            output.vy.data[3] = vy.Dot(mat.vw);

            output.vz.data[0] = vz.Dot(mat.vx);
            output.vz.data[1] = vz.Dot(mat.vy);
            output.vz.data[2] = vz.Dot(mat.vz);
            output.vz.data[3] = vz.Dot(mat.vw);

            output.vw.data[0] = vw.Dot(mat.vx);
            output.vw.data[1] = vw.Dot(mat.vy);
            output.vw.data[2] = vw.Dot(mat.vz);
            output.vw.data[3] = vw.Dot(mat.vw);

            return output;
        }

        Vector4 operator*(const Vector4& other) const
        {
            float x = vx.Dot(other);
            float y = vy.Dot(other);
            float z = vz.Dot(other);
            float w = vw.Dot(other);

            return Vector4(x, y, z, w);
        }

        void operator+=(const Matrix4x4& other)
        {
            vx += other.vx;
            vy += other.vy;
            vz += other.vz;
            vw += other.vw;
        }
        void operator-=(const Matrix4x4& other)
        {
            vx -= other.vx;
            vy -= other.vy;
            vz -= other.vz;
            vw -= other.vw;
        }
        void operator*=(const Matrix4x4& other)
        {
            Matrix4x4 mat = other;
            mat.Transpose();
            Matrix4x4 copy = *this;

            vx.data[0] = copy.vx.Dot(mat.vx);
            vx.data[1] = copy.vx.Dot(mat.vy);
            vx.data[2] = copy.vx.Dot(mat.vz);
            vx.data[3] = copy.vx.Dot(mat.vw);

            vy.data[0] = copy.vy.Dot(mat.vx);
            vy.data[1] = copy.vy.Dot(mat.vy);
            vy.data[2] = copy.vy.Dot(mat.vz);
            vy.data[3] = copy.vy.Dot(mat.vw);

            vz.data[0] = copy.vz.Dot(mat.vx);
            vz.data[1] = copy.vz.Dot(mat.vy);
            vz.data[2] = copy.vz.Dot(mat.vz);
            vz.data[3] = copy.vz.Dot(mat.vw);

            vw.data[0] = copy.vw.Dot(mat.vx);
            vw.data[1] = copy.vw.Dot(mat.vy);
            vw.data[2] = copy.vw.Dot(mat.vz);
            vw.data[3] = copy.vw.Dot(mat.vw);
        }

        void Identity()
        {
            vx = { 1.0f, 0.0f, 0.0f, 0.0f };
            vy = { 0.0f, 1.0f, 0.0f, 0.0f };
            vz = { 0.0f, 0.0f, 1.0f, 0.0f };
            vw = { 0.0f, 0.0f, 0.0f, 1.0f };
        }
        void Transpose()
        {
            Matrix4x4 temp = *this;
            vx = { temp.vx.data[0], temp.vy.data[0], temp.vz.data[0], temp.vw.data[0] };
            vy = { temp.vx.data[1], temp.vy.data[1], temp.vz.data[1], temp.vw.data[1] };
            vz = { temp.vx.data[2], temp.vy.data[2], temp.vz.data[2], temp.vw.data[2] };
            vw = { temp.vx.data[3], temp.vy.data[3], temp.vz.data[3], temp.vw.data[3] };
        }
        void SetOrtho(float width, float height, float near, float far)
        {
            //The following docs are in row major, this matrix uses column major
            //https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixortholh Left handed, USE_LEFT_HANDED_MATRICES = true
            //https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixorthorh Right handed, USE_LEFT_HANDED_MATRICES = false
            Identity();
            if constexpr (USE_LEFT_HANDED_MATRICES)
            {
                vx.data[0] = 2 / width;
                vy.data[1] = 2 / height;
                vz.data[2] = 1 / (far - near);
                vz.data[3] = -near / (far - near);
            }
            else
            {
                vx.data[0] = 2 / width;
                vy.data[1] = 2 / height;
                vz.data[2] = 1 / (near - far);
                vz.data[3] = near / (near - far);
            }
        }
        void SetPerspective(float fovAngleY, float aspectRatio, float near, float far)
        {
            //The following docs are in row major, this matrix uses column major
            //https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh Left Handed, posZIn = true
            //https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovrh Right handed, posZIn = false
            float yScale = 1 / tanf(fovAngleY * 3.14f / 180.0f * 0.5f);
            float xScale = yScale / aspectRatio;

            Identity();
            if constexpr (USE_LEFT_HANDED_MATRICES)
            {
                vx.data[0] = xScale;
                vy.data[1] = yScale;
                vz.data[2] = far / (far - near);
                vz.data[3] = (-near * far) / (far - near);
                vw.data[2] = 1;
                vw.data[3] = 0;
            }
            else
            {
                vx.data[0] = xScale;
                vy.data[1] = yScale;
                vz.data[2] = far / (near - far);
                vz.data[3] = (near * far) / (near - far);
                vw.data[2] = -1;
                vw.data[3] = 0;
            }
        }

        void SetPosition(Vector3 position)
        {
            vx.data[3] = position.x;
            vy.data[3] = position.y;
            vz.data[3] = position.z;
        }
        void Translate(Vector3 position)
        {
            vx.data[3] += position.x;
            vy.data[3] += position.y;
            vz.data[3] += position.z;
        }
        void RotateX(float degrees)
        {
            Matrix4x4 rotMatrix;
            float radians = degrees / 180.f * static_cast<float>(PI);

            float sinAngle = sinf(radians);
            float cosAngle = cosf(radians);

            rotMatrix.vy.data[1] = cosAngle;
            rotMatrix.vy.data[2] = sinAngle;
            rotMatrix.vz.data[1] = -sinAngle;
            rotMatrix.vz.data[2] = cosAngle;

            *this *= rotMatrix;
        }
        void RotateY(float degrees)
        {
            Matrix4x4 rotMatrix;
            float radians = degrees / 180.f * static_cast<float>(PI);

            float sinAngle = sinf(radians);
            float cosAngle = cosf(radians);

            rotMatrix.vx.data[0] = cosAngle;
            rotMatrix.vx.data[2] = sinAngle;
            rotMatrix.vz.data[0] = -sinAngle;
            rotMatrix.vz.data[2] = cosAngle;

            *this *= rotMatrix;
        }
        void RotateZ(float degrees)
        {
            Matrix4x4 rotMatrix;
            float radians = degrees / 180.f * static_cast<float>(PI);

            float sinAngle = sinf(radians);
            float cosAngle = cosf(radians);

            rotMatrix.vx.data[0] = cosAngle;
            rotMatrix.vx.data[1] = -sinAngle;
            rotMatrix.vy.data[0] = sinAngle;
            rotMatrix.vy.data[1] = cosAngle;

            *this *= rotMatrix;
        }
        void SetScale(Vector3 scale)
        {
            vx.data[0] = scale.x;
            vy.data[1] = scale.y;
            vz.data[2] = scale.z;
        }
        void Scale(Vector3 scale)
        {
            vx.data[0] *= scale.x;
            vy.data[1] *= scale.y;
            vz.data[2] *= scale.z;
        }

        Vector3 GetPosition() { return Vector3(vx.data[3], vy.data[3], vz.data[3]); }
        Vector3 GetScale() { return Vector3(vx.data[0], vy.data[1], vz.data[2]); }
        Vector3 GetEulerAngles()
        {
            float radToDeg = 180 / static_cast<float>(PI);
            if (vy.data[2] > 1.0f)
            {
                float x = static_cast<float>(PI) / 2;
                float y = atan2f(vx.data[1], vx.data[0]);
                float z = 0.0f;
                return Vector3(x, y, z) * radToDeg;
            }
            else if (vy.data[2] < -1.0f)
            {
                float x = static_cast<float>(PI) / 2;
                float y = -atan2f(vx.data[1], vx.data[0]);
                float z = 0.0f;
                return Vector3(x, y, z) * radToDeg;
            }
            else
            {
                float x = asinf(vy.data[2]);
                float y = atan2f(-vx.data[2], vz.data[2]);
                float z = atan2f(-vy.data[0], vy.data[1]);
                return Vector3(x, y, z) * radToDeg;
            }
        }

        Matrix4x4 GetTransposed()
        {
            Matrix4x4 temp = *this;
            temp.Transpose();
            return temp;
        }

        static Matrix4x4 PositionMatrix(Vector3 position)
        {
            Matrix4x4 matrix;
            matrix.SetPosition(position);
            return matrix;
        }
        static Matrix4x4 ScaleMatrix(Vector3 scale)
        {
            Matrix4x4 matrix;
            matrix.SetScale(scale);
            return matrix;
        }

    };
}
#ifndef near
#define near
#endif // near

#ifndef far
#define far
#endif // far