#pragma once
#include "Component.h"
#include <vector>
#include "CommonsLibrary/Math/Vector3.h"
#include "CommonsLibrary/Math/Quaternion.h"

namespace CommonsLibrary
{
    class Transform final : public Component
    {
        ECS_COMPONENT_SETUP(Transform, Component)

        friend class ComponentManager;
        friend class GameObjectManager;
    private:
        ReferencePointer<Transform> m_parent;
        std::vector<ReferencePointer<Transform>> m_children;

        Vector3 m_position;
        Vector3 m_scale;
        Quaternion m_rotation;

    public:
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
        void Awake() final;

    public:
        void SetParent(ReferencePointer<Transform> parent);

        void SetPosition(Vector3 position) { SetLocalPosition(position - GetPosition()); }
        void SetLocalPosition(Vector3 position) { m_position = position; }
        void SetLocalScale(Vector3 scale) { m_scale = scale; }
        void SetLocalRotation(Quaternion rotation) { m_rotation = rotation; }

        Vector3 GetPosition() { return (m_parent) ? m_parent->GetPosition() : GetLocalPosition(); }
        Vector3 GetLocalPosition() { return m_position; }
        Vector3 GetLocalScale() { return m_scale; }
        Quaternion GetLocalRotation() { return m_rotation; }
        Matrix4x4 GetTransformMatrix();

        ReferencePointer<Transform> GetParent() const { return m_parent; }
        ReferencePointer<Transform> GetChild(unsigned int index) const { return m_children[index]; }
        std::vector<ReferencePointer<Transform>> GetChildren() const { return m_children; }

        Vector3 Forward() { return (m_rotation.Matrix() * Matrix4x4::PositionMatrix(Vector3::Forward())).GetPosition(); }
        Vector3 Up() { return (m_rotation.Matrix() * Matrix4x4::PositionMatrix(Vector3::Up())).GetPosition(); }
        Vector3 Right() { return (m_rotation.Matrix() * Matrix4x4::PositionMatrix(Vector3::Right())).GetPosition(); }

        Vector3 Backward() { return -Forward(); }
        Vector3 Down() { return -Up(); }
        Vector3 Left() { return -Right(); }


    private:
        void AddChild(ReferencePointer<Transform> child);
        void RemoveChild(ReferencePointer<Transform> child);

    private:
        bool IsHeirarchyActive();
    };
}