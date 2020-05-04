#pragma once
#include "Component.h"
#include <vector>
#include "CommonsLibrary/Math/Vector3.h"
#include "CommonsLibrary/Math/Quaternion.h"

namespace CommonsLibrary
{
    class GameObject;

    class Transform final 
    {
    public:
        Vector3 position;
        Vector3 scale;
        Quaternion rotation;

    public:
        Matrix4x4 GetMatrix() const { return rotation.Matrix() * Matrix4x4::ScaleMatrix(scale) * Matrix4x4::PositionMatrix(position); }

        Vector3 Forward() const { return (rotation.Matrix() * Matrix4x4::PositionMatrix(Vector3::Forward())).GetPosition(); }
        Vector3 Up() const { return (rotation.Matrix() * Matrix4x4::PositionMatrix(Vector3::Up())).GetPosition(); }
        Vector3 Right() const { return (rotation.Matrix() * Matrix4x4::PositionMatrix(Vector3::Right())).GetPosition(); }

        Vector3 Backward() const { return -Forward(); }
        Vector3 Down() const { return -Up(); }
        Vector3 Left() const { return -Right(); }

    };
}