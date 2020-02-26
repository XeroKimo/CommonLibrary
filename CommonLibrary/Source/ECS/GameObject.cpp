#include "ECS/GameObject.h"
#include "ECS/World.h"
#include "DebugTools/Logger.h"

namespace CommonsLibrary
{
	GameObject::GameObject(const ReferencePointer<World>& world)
	{
		m_world = world;

		m_transform = AddComponent<Transform>();
	}

	void CommonsLibrary::GameObject::Start()
	{
		for (const ReferencePointer<Component>& component : m_activeComponents)
		{
#ifdef _DEBUG
			try
			{
				component->Start();
			}
			catch (std::exception e)
			{
				Logger::Instance()->Log(e.what());
			}
#else
			component->Start();
#endif
		}
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

		auto it = std::find(componentVector.begin(), componentVector.end(), component);
		if (it != componentVector.end())
			componentVector.erase(it);

		if (KeyExists(m_components, key))
		{
			m_components.erase(key);
		}
		else
		{
			for (auto componentPair : m_components)
			{
				std::vector<ReferencePointer<Component>>* searchComponents = &componentPair.second;
				auto it = std::find(searchComponents->begin(), searchComponents->end(), component);
				if (it != searchComponents->end())
				{
					searchComponents->erase(it);
					return;
				}
			}
		}
	}

	void CommonsLibrary::GameObject::SetComponentActive(const ReferencePointer<Component>& component)
	{
		std::vector<ReferencePointer<Component>>& mapToSearchIn = (component->IsActive()) ? m_activeComponents : m_inactiveComponents; 
		std::vector<ReferencePointer<Component>>& mapToMoveComponent = (component->IsActive()) ? m_inactiveComponents : m_activeComponents;

		auto findIt = std::find(mapToSearchIn.begin(), mapToSearchIn.end(), component);
		mapToSearchIn.erase(findIt);
		mapToMoveComponent.push_back(component);
	}
}