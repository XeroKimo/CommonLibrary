#include "ECS/GameObject.h"
#include "ECS/World.h"
#include "DebugTools/Logger.h"
#include "VectorHelpers.h"
#include "ECS/Scene.h"

namespace CommonsLibrary
{
	GameObject::GameObject(const ReferencePointer<World>& world, Scene* const scene)
	{
		m_world = world;
		m_owningScene = scene;

		m_transform = AddComponent<Transform>();
		m_activeInHeirarchy = m_activeInWorld = true;
        name = "GameObject";
	}

	void CommonsLibrary::GameObject::Update(float deltaTime)
	{
		for (const ReferencePointer<Component>& component : m_activeComponents)
		{
#ifdef _DEBUG
			try
			{
				component->Update(deltaTime);
			}
			catch (std::exception e)
			{
				Logger::Instance()->Log(e.what());
			}
#else
			component->Update(deltaTime);
#endif
		}
	}

	void CommonsLibrary::GameObject::RemoveComponent(ReferencePointer<Component> component)
	{
		if (!component)
			return;
		if (component->GetGameObject() != GetReferencePointer())
			return;

		std::type_index key(typeid(*component));

		std::vector<ReferencePointer<Component>>& componentVector = (component->IsActive()) ? m_activeComponents : m_inactiveComponents;
		if (component->IsActive() && !component->m_hasStarted)
			m_world->RemoveComponentToStart(component.Get());

		RemoveFromVector(componentVector, component);
		if (KeyExists(m_components, key))
		{
			RemoveFromVector(m_components[key], component);
		}
		else
		{
			for (auto componentPair : m_components)
			{
				std::vector<ReferencePointer<Component>>* searchComponents = &componentPair.second;
				RemoveFromVector(*searchComponents, component);
			}
		}
	}

	void GameObject::SetIsActive(bool active)
	{
		if (active == m_activeInHeirarchy)
			return;

		m_activeInHeirarchy = active;
		SetIsActiveInWorld();
	}

	void CommonsLibrary::GameObject::SetComponentActive(const ReferencePointer<Component>& component)
	{
		std::vector<ReferencePointer<Component>>& arrayToSearchIn = (component->IsActive()) ? m_activeComponents : m_inactiveComponents; 
		std::vector<ReferencePointer<Component>>& arrayToMoveIn = (component->IsActive()) ? m_inactiveComponents : m_activeComponents;

		RemoveFromVector(arrayToSearchIn, component);
		arrayToMoveIn.push_back(component);
		if (!component->m_hasStarted)
			m_world->AddComponentToStart(component.Get());
	}
	void GameObject::AddComponentToStart(Component* component)
	{
		m_world->AddComponentToStart(component);
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
		m_world->SetObjectActive(GetReferencePointer());
		if (!m_activeInWorld)
		{
			for (const ReferencePointer<Component>& component : m_activeComponents)
			{
				if (!component->m_hasStarted)
					m_world->RemoveComponentToStart(component.Get());
			}
		}
	}
}