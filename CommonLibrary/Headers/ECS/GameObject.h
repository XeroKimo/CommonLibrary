#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "UnorderedMapHelpers.h"
#include "Transform.h"
#include <typeindex>


namespace CommonsLibrary
{
	class Transform;
	class Scene;
	class GameObject final : public ReferenceFromThis<GameObject>
	{
		friend class Component;
		friend class Scene;
		friend class Transform;
	private:
		std::vector<Component*> m_componentsToStart;
		std::vector<Component*> m_activeComponents;
		std::vector<Component*> m_inactiveComponents;

		std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>> m_componentMap;

		ReferencePointer<Transform> m_transform;
		Scene* m_scene;

		bool m_activeInWorld;
		bool m_activeInHeirarchy;

		bool m_isStart;
		bool m_isDead;
	public:
		std::string name;
	public:
		GameObject(Scene* const scene);

	private:
		void Start();
		void Update(float deltaTime);
		void OnDestroy();

	public:
		template<class Type, class = std::enable_if_t<std::is_base_of_v<Component, Type>>>
		ReferencePointer<Type> AddComponent()
		{
            static_assert(std::is_convertible<std::remove_cv_t<Type>*, typename Component::RefThis*>::value, "Multiple inheritance of type Component is not allowed");
			std::type_index key(typeid(Type));
			auto& components = m_componentMap[key];

			ReferencePointer<Component> createdComponent = ComponentRegistry::Create(key, GetReferencePointer());
			if (createdComponent->IsActive())
			{
				m_activeComponents.push_back(createdComponent.Get());
				m_componentsToStart.push_back(createdComponent.Get());
				SetGameObjectToStart();
			}
			else
			{
				m_inactiveComponents.push_back(createdComponent.Get());
			}
			components.push_back(std::move(createdComponent));

			return ReferencePointerStaticCast<Type>(components.back());
		}

		void RemoveComponent(ReferencePointer<Component> component);

		template<class Type>
		ReferencePointer<Type> GetComponent() const
		{
			std::type_index key(typeid(Type));
			ReferencePointer<Type> component = GetComponent<Type>(m_componentMap, key);
			return component;
		}

		template<class Type>
		std::vector<Type> GetComponents() const
		{
			std::type_index key(typeid(Type));
			std::vector<Type> components;
			GetComponents<Type>(m_componentMap, key, components);
			return components;
		}

		ReferencePointer<Transform> GetTransform() { return m_transform; }

	public:
		void SetIsActive(bool active);
		bool IsActiveInHeirarchy() { return m_activeInHeirarchy; }
		bool IsActiveInWorld() { return m_activeInWorld; }

	public:
		Scene* GetScene() { return m_scene; } 

	private:
        void InitTransform();
		void SetGameObjectToStart();
		void SetComponentActive(const ReferencePointer<Component>& component);

		template<class Type>
		ReferencePointer<Type> GetComponent(const std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>>& componentMap, const std::type_index& key) const
		{
			if (KeyExists(componentMap, key))
			{
				return ReferencePointerStaticCast<Type>(componentMap[key][0]);
			}
			else
			{
				ReferencePointer<Type> type;
				for (const auto& componentPair : componentMap)
				{
					type = ReferencePointerDynamicCast<Type>(componentPair.second[0]);
					if (type)
						return type;
				}
				return nullptr;
			}
		}

		template<class Type>
		void GetComponents(const std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>>& componentMap, const std::type_index& key, std::vector<Type>& components) const
		{
			for (const auto& componentPair : componentMap)
			{
				if (componentPair.first == key)
				{
					for (ReferencePointer<Component> component : componentPair.second)
					{
						components.push_back(ReferencePointerStaticCast<Type>(component));
					}
				}
				else
				{
					ReferencePointer<Type> castComponent = ReferencePointerDynamicCast<Type>(componentPair.second[0]);
					if (castComponent)
					{
						components.push_back(castComponent);
						for (int i = 1; i < componentPair.second.size(); i++)
						{
							components.push_back(ReferencePointerStaticCast<Type>(componentPair.second[i]));
						}
					}
				}
			}
		}

		void SetChildrenActiveInWorld();
		bool IsParentActiveInWorld();
		void SetIsActiveInWorld();
	};
}