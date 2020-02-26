#pragma once
#include "Component.h"
#include <vector>
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

namespace CommonsLibrary
{
    class Transform : public Component
    {
    private:
        ReferencePointer<Transform> m_parent;
        std::vector<ReferencePointer<Transform>> m_children;

        Vector3 m_position;
        Vector3 m_scale;
        Quaternion m_rotation;
    public:
        //Transform() = default;
        Transform(const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world) : Component(gameObject, world) {}

    public:
        // Inherited via Component
        virtual void Start() override;
        virtual void Update(float deltaTime) override;

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

        ReferencePointer<Transform> GetChild(unsigned int index) { return m_children[index]; }
        std::vector<ReferencePointer<Transform>> GetChildren() { return m_children; }
    private:
        void AddChild(ReferencePointer<Transform> child);
        void RemoveChild(ReferencePointer<Transform> child);
    };
}