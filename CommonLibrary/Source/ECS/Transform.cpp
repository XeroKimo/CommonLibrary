#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/Transform.h"
#include "CommonsLibrary/ECS/GameObject.h"

namespace CommonsLibrary
{
    Matrix4x4 Transform::GetTransformMatrix()
    {
        if(m_requiresRecache)
        {
            m_cachedTransform = m_rotation.Matrix() * Matrix4x4::ScaleMatrix(m_scale) * Matrix4x4::PositionMatrix(m_position);
            if(m_owner->GetParent())
                m_cachedTransform = m_owner->GetParent()->GetTransform().GetTransformMatrix() * m_cachedTransform;

            m_requiresRecache = false;
        }

        return m_cachedTransform;
    }

    Vector3 Transform::AccumulateWorldPosition(Vector3 position) const
    {
        if(m_owner->GetParent())
            return m_owner->GetParent()->GetTransform().AccumulateWorldPosition(position);
        return m_position + position;
    }

    void Transform::SetFlagForChildren()
    {
        if(m_requiresRecache)
            return;

        m_requiresRecache = true;
        auto children = m_owner->GetChildren();
        for(auto child : children)
        {
            child->GetTransform().SetFlagForChildren();
        }
    }
}