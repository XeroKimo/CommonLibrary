#include "ECS/Component.h"
#include "ECS/World.h"
#include "ECS/GameObject.h"

namespace CommonsLibrary
{
    Component::Component(const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world) :
        m_world(world),
        m_gameObject(gameObject)
    {
    }
    void Component::SetActive(bool active)
    {
        if (m_active == active)
            return;

        m_gameObject->SetComponentActive(GetReferencePointer());
		m_active = active;
    }
	ReferencePointer<GameObject> Component::GetGameObject()
	{
		return m_gameObject;
	}
}