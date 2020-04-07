#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/Component.h"

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
        if (!active)
            return;
        if (m_hasStarted)
            return;
        Start();
        m_hasStarted = true;
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

    ReferencePointer<Transform> Component::GetTransform() const
    {
        return m_gameObject->GetTransform();
    }

    ReferencePointer<GameObject> Component::CreateGameObject()
    {
        return GetGameObject()->CreateGameObject();
    }
}