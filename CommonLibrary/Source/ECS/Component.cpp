#include "ECS/Component.h"
#include "ECS/GameObject.h"
#include "ECS/Scene.h"

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
        return GetGameObject()->GetScene();
    }
    World* Component::GetWorld()
    {
        return GetGameObject()->GetScene()->GetWorld();
    }
}