#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/Component.h"

namespace CommonsLibrary
{
    Component::Component(const ReferencePointer<GameObject>& owningGameObject) :
        m_gameObject(owningGameObject)
    {

    }

    void Component::SetActive(bool active)
    {
        if(m_isDestroyed)
            return;
        m_gameObject->SetComponentActive(GetReferencePointer(), active);
    }
}