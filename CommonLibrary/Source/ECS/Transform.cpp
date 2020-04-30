#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/Transform.h"
#include "CommonsLibrary/ECS/GameObject.h"

namespace CommonsLibrary
{
    Matrix4x4 Transform::GetTransformMatrix() const
    {
        return m_rotation.Matrix() * Matrix4x4::ScaleMatrix(m_scale) * Matrix4x4::PositionMatrix(m_position);
    }
    Vector3 Transform::AccumulateWorldPosition(Vector3 position) const
    {
        if(m_owner->GetParent())
            return m_owner->GetParent()->GetTransform()->AccumulateWorldPosition(position + GetLocalPosition());
        return position + GetLocalPosition();
    }
}