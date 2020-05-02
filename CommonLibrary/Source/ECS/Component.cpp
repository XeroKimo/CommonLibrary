#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/Component.h"

namespace CommonsLibrary
{
    Component::Component(const ReferencePointer<GameObject>& owningGameObject) :
        m_gameObject(owningGameObject)
    {

    }

    void Component::CopyComponent(const Component* const other)
    {
        m_active = other->m_active;
    }

    void Component::SetActive(bool active)
    {
        m_gameObject->SetComponentActive(GetReferencePointer(), active);
    }
}