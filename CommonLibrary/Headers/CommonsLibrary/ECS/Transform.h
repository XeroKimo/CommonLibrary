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
        friend class ComponentManager;
        friend class GameObjectManager;
    private:

        GameObject* m_owner;
        Vector3 m_position;
        Vector3 m_scale;
        Quaternion m_rotation;

    public:
        Transform(GameObject* owner) : m_owner(owner) {}
        Transform(const Transform& other) = delete;
        Transform(Transform&& other) = delete;

    private:
        Transform operator=(const Transform& other)
        {
            m_position = other.m_position;
            m_scale = other.m_scale;
            m_rotation = other.m_rotation;
        }
        Transform& operator=(Transform&& other) = delete;


    public:
        void SetPosition(Vector3 position) { SetLocalPosition(position - GetPosition()); }
        void SetLocalPosition(Vector3 position) { m_position = position; }
        void SetLocalScale(Vector3 scale) { m_scale = scale; }
        void SetRotation(Quaternion rotation) { m_rotation = rotation; }

        Vector3 GetPosition() const { return AccumulateWorldPosition(Vector3()); }
        Vector3 GetLocalPosition() const { return m_position; }
        Vector3 GetLocalScale() const { return m_scale; }
        Quaternion GetRotation() const { return m_rotation; }
        Matrix4x4 GetTransformMatrix() const;

        Vector3 Forward() const { return (m_rotation.Matrix() * Matrix4x4::PositionMatrix(Vector3::Forward())).GetPosition(); }
        Vector3 Up() const { return (m_rotation.Matrix() * Matrix4x4::PositionMatrix(Vector3::Up())).GetPosition(); }
        Vector3 Right() const { return (m_rotation.Matrix() * Matrix4x4::PositionMatrix(Vector3::Right())).GetPosition(); }

        Vector3 Backward() const { return -Forward(); }
        Vector3 Down() const { return -Up(); }
        Vector3 Left() const { return -Right(); }

    private:
        Vector3 AccumulateWorldPosition(Vector3 position) const;
    };
}