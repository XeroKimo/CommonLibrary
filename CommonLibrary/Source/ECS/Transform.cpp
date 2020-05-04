#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/Transform.h"
#include "CommonsLibrary/ECS/GameObject.h"

namespace CommonsLibrary
{
    Matrix4x4 Transform::GetLocalMatrix() const
    {
        return m_rotation.Matrix() * Matrix4x4::ScaleMatrix(m_scale) * Matrix4x4::PositionMatrix(m_position);
    }

    Matrix4x4 Transform::GetWorldMatrix() const
    {
        if(GetParent())
            return m_gameObject->GetParent()->GetTransform().GetWorldMatrix() * GetLocalMatrix();
        return GetLocalMatrix();
    }

    Vector3 Transform::AccumulateWorldPosition(Vector3 position) const
    {
        if(GetParent())
            return GetParent()->GetTransform().AccumulateWorldPosition(position);
        return m_position + position;
    }
    GameObject* Transform::GetParent() const
    {
        return m_gameObject->GetParent().Get();
    }
}