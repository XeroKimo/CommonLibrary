#pragma once
#include "Component.h"
#include <vector>
#include "CommonsLibrary/Math/Vector3.h"
#include "CommonsLibrary/Math/Quaternion.h"

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
        Transform(const ReferencePointer<IGameObject>& gameObject) : Component(gameObject) {}

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

        ReferencePointer<Transform> GetParent() const { return m_parent; }
        ReferencePointer<Transform> GetChild(unsigned int index) const { return m_children[index]; }
        std::vector<ReferencePointer<Transform>> GetChildren() const { return m_children; }
    private:
        void AddChild(ReferencePointer<Transform> child);
        void RemoveChild(ReferencePointer<Transform> child);
    };
}