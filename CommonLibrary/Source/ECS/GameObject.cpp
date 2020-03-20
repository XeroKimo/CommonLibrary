#include "ECS/GameObject.h"
#include "ECS/World.h"
#include "DebugTools/Logger.h"
#include "VectorHelpers.h"
#include "ECS/Scene.h"

namespace CommonsLibrary
{
	GameObject::GameObject(Scene* const scene) :
		m_scene(scene),
		m_activeInHeirarchy(true),
		m_activeInWorld(true),
		name("GameObject"),
		m_isStart(false)
	{
	}

	void GameObject::Start()
	{
		if (!m_componentsToStart.empty())
		{
			for (const auto components : m_componentsToStart)
				components->Start();
			m_componentsToStart.clear();
			m_isStart = false;
		}
	}

	void GameObject::Update(float deltaTime)
	{
		for (const auto component : m_activeComponents)
			component->Update(deltaTime);
	}

	void GameObject::OnDestroy()
	{
		for (const auto component : m_activeComponents)
			component->OnDestroy();
	}

	void CommonsLibrary::GameObject::RemoveComponent(ReferencePointer<Component> component)
	{
		if (!component)
			return;
		if (component->GetGameObject() != GetReferencePointer())
			return;

		std::type_index key(typeid(*component));

		std::vector<Component*>& componentVector = (component->IsActive()) ? m_activeComponents : m_inactiveComponents;

		RemoveFromVector(componentVector, component.Get());
		if (KeyExists(m_componentMap, key))
		{
			RemoveFromVector(m_componentMap[key], component);
		}
		else
		{
			for (auto componentPair : m_componentMap)
			{
				std::vector<ReferencePointer<Component>>* searchComponents = &componentPair.second;
				RemoveFromVector(*searchComponents, component);
			}
		}
	}

	ReferencePointer<Transform> GameObject::GetTransform()
	{
		if (!m_transform)
		{
			ReferencePointer<Transform> transform = GetComponent<Transform>();
			if (transform)
				m_transform = transform;
			else
				m_transform = AddComponent<Transform>();
		}
		return m_transform;
	}

	void GameObject::SetIsActive(bool active)
	{
		if (active == m_activeInHeirarchy)
			return;

		m_activeInHeirarchy = active;
		SetIsActiveInWorld();
	}

    void GameObject::InitTransform()
    {
        m_transform = AddComponent<Transform>();
    }

	void GameObject::SetGameObjectToStart()
	{
		if (m_isStart)
			return;

		m_scene->SetGameObjectToStart(GetReferencePointer());
	}

    void CommonsLibrary::GameObject::SetComponentActive(const ReferencePointer<Component>& component)
	{
		std::vector<Component*>& arrayToSearchIn = (component->IsActive()) ? m_activeComponents : m_inactiveComponents; 
		std::vector<Component*>& arrayToMoveIn = (component->IsActive()) ? m_inactiveComponents : m_activeComponents;

		RemoveFromVector(arrayToSearchIn, component.Get());
		arrayToMoveIn.push_back(component.Get());
		if (!component->m_hasStarted)
			m_componentsToStart.push_back(component.Get());
	}
	void GameObject::SetChildrenActiveInWorld()
	{
		for (const ReferencePointer<Transform>& child : m_transform->GetChildren())
		{
			child->GetGameObject()->SetChildrenActiveInWorld();
			child->GetGameObject()->SetIsActiveInWorld();
		}
	}
	bool GameObject::IsParentActiveInWorld()
	{
		ReferencePointer<Transform> parent = GetTransform()->GetParent();
		if (parent)
		{
			if (!parent->GetGameObject()->m_activeInHeirarchy)
				return false;
			else
				return parent->GetGameObject()->IsParentActiveInWorld();
		}
		else
		{
			return m_activeInHeirarchy;
		}
	}
	void GameObject::SetIsActiveInWorld()
	{
		bool active = IsParentActiveInWorld() & m_activeInHeirarchy;
		if (active == m_activeInWorld)
			return;
		m_activeInWorld = active;
		//m_world->SetObjectActive(GetReferencePointer());

	}
}