#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/Component.h"

namespace CommonsLibrary
{

    void Component::CopyComponent(const Component* const other)
    {
        m_active = other->m_active;
    }

    void Component::SetActive(bool active)
    {
        m_gameObject->SetComponentActive(GetReferencePointer(), active);
    }
}