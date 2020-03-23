#include "CommonsLibrary/ECS/Component.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/ECS/Scene.h"

namespace CommonsLibrary
{
    Component::Component(const ReferencePointer<IGameObject>& gameObject) :
        m_gameObject(gameObject),
        m_active(true),
        m_hasStarted(false)
    {
    }
    void Component::SetActive(bool active)
    {
        if (m_active == active)
            return;

        //static_cast<GameObject*>(GetGameObject().Get())->SetComponentActive(GetReferencePointer());
		m_active = active;
    }
    ReferencePointer<IGameObject> Component::GetGameObject()
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