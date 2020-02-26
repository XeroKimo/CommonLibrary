#include "ECS/Component.h"
//#include "ECS/World.h"
#include "ECS/GameObject.h"

namespace CommonsLibrary
{
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