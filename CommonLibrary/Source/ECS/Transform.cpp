#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/Transform.h"
#include "CommonsLibrary/ECS/GameObject.h"

namespace CommonsLibrary
{
    ECS_COMPONENT_REGISTER(Transform)

    void Transform::Awake()
    {
        SetActive(false);
    }

    void Transform::CopyComponent(const Component* const other)
    {
        const Transform* copy = static_cast<const Transform*>(other);
        m_position = copy->m_position;
        m_rotation = copy->m_rotation;
        m_scale = copy->m_scale;
    }

    void Transform::SetParent(ReferencePointer<Transform> parent)
    {
        if (m_parent)
            m_parent->RemoveChild(ReferencePointerStaticCast<Transform>(GetReferencePointer()));

        m_parent = parent;

        if (m_parent)
            m_parent->AddChild(ReferencePointerStaticCast<Transform>(GetReferencePointer()));

        GetGameObject()->SetActiveWorld(IsHeirarchyActive());
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
    bool Transform::IsHeirarchyActive()
    {
        if(!m_parent)
            return GetGameObject()->IsActiveInHeirarchy();
        if(!m_parent->GetGameObject()->IsActiveWorld())
            return false;
        return m_parent->IsHeirarchyActive();
    }
}