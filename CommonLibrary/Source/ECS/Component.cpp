#include "CommonsLibrary/ECS/Component.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/ECS/Scene.h"

namespace CommonsLibrary
{
    Component::Component(const ReferencePointer<GameObject>& gameObject) :
        m_gameObject(gameObject),
        m_active(true),
        m_hasStarted(false)
    {
    }
    void Component::SetActive(bool active)
    {
        if (m_active == active)
            return;

        GetGameObject()->SetComponentActive(GetReferencePointer());
		m_active = active;
    }
    ReferencePointer<GameObject> Component::GetGameObject()
    {
        return m_gameObject;
    }
    Scene* Component::GetScene()
    {
        return nullptr;
    }
    World* Component::GetWorld()
    {
        return nullptr;
    }
}