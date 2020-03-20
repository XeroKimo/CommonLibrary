#include "CommonsLibrary/ECS/Transform.h"
#include "CommonsLibrary/ECS/GameObject.h"

namespace CommonsLibrary
{
    void Transform::Start()
    {
    }

    void Transform::Update(float deltaTime)
    {
    }

    void Transform::SetParent(ReferencePointer<Transform> parent)
    {
        if (m_parent)
        {
            m_parent->RemoveChild(ReferencePointerStaticCast<Transform>(GetReferencePointer()));
        }
        m_parent = parent;
        if (m_parent)
        {
            m_parent->AddChild(ReferencePointerStaticCast<Transform>(GetReferencePointer()));
            GetGameObject()->SetIsActiveInWorld();
        }
    }

    Matrix4x4 Transform::GetTransformMatrix()
    {
        return m_rotation.Matrix() * Matrix4x4::ScaleMatrix(m_scale) * Matrix4x4::PositionMatrix(m_position);
    }
    void Transform::AddChild(ReferencePointer<Transform> child)
    {
        m_children.push_back(child);
    }
    void Transform::RemoveChild(ReferencePointer<Transform> child)
    {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end())
            m_children.erase(it);
    }
}