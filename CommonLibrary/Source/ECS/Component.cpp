#include "CommonsLibrary/ECS/Component.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/ECS/Scene.h"

namespace CommonsLibrary
{
    Component::Component(ReferencePointer<GameObject> owningGameObject) :
        m_gameObject(owningGameObject),
        active(true),
        m_hasStarted(false)
    {

    }

    void Component::StartComponent()
    {
        if (m_hasStarted)
            return;
        Start();
    }

    void Component::UpdateComponent(float deltaTime)
    {
        if (!active)
            return;

        Update(deltaTime);
    }

    void Component::DestroyComponent()
    {
        OnDestroy();
    }

    ReferencePointer<GameObject> Component::CreateGameObject()
    {
        return GetGameObject()->CreateGameObject();
    }

    World* Component::GetWorld()
    {
        return m_gameObject->m_scene->GetWorld();
    }

}