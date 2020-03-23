#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "CommonsLibrary/StdHelpers/UnorderedMapHelpers.h"
#include "Transform.h"
#include "ComponentMap.h"
#include "UpdateableComponents.h"
#include <typeindex>

namespace CommonsLibrary
{
	class Transform;
	class Scene;
	class GameObject : public ReferenceFromThis<GameObject>
	{
		friend class Component;
		friend class Transform;
	protected:
		Scene* m_scene;
		ReferencePointer<Transform> m_transform;
		ComponentMap m_componentMap;
		UpdateableComponents m_updateableComponents;	

		bool m_activeInWorld;
		bool m_activeInHeirarchy;

		bool m_hasComponentToStart;
		bool m_hasComponentToRemove;
		bool m_isDestroyed;
	public:
		std::string name;

	public:
		GameObject(Scene* const scene);

	public:
		void SetIsActive(bool active);
		bool GetActiveWorld() { return m_activeInWorld; }
		bool GetActiveHeirarchy() { return m_activeInHeirarchy; }

	public:
		void Destroy();

	public:
		template <class Type, class Enable = std::enable_if_t<std::is_base_of_v<Component, Type>>>
		ReferencePointer<Type> AddComponent()
		{
			ReferencePointer<Type> component = m_componentMap.AddComponent<Type>(GetReferencePointer());
			m_updateableComponents.AddComponent(component.Get());
			return component;
		}

		template <class Type>
		ReferencePointer<Type> GetComponent()
		{
			return m_componentMap.GetComponent<Type>();
		}

		template <class Type>
		std::vector<ReferencePointer<Type>> GetComponents()
		{
			return m_componentMap.GetComponents<Type>();
		}

		void RemoveComponent(const ReferencePointer<Component>& component);

		ReferencePointer<Transform> GetTransform();
	private:
		void SetComponentActive(const ReferencePointer<Component>& component);

	private:
		void AddGameObjectToStart();
		void AddGameObjectToCleanUp();

		void SetChildrenActiveInWorld();
		bool IsParentActiveInWorld();
		void SetActiveInWorld(bool active);
	};

	class UpdateableGameObject final : public GameObject
	{
	public:
		UpdateableGameObject(Scene* const scene);
		~UpdateableGameObject() = default;
	public:
		void Start();
		void Update(float deltaTime);
		void CleanUpComponents();
		void OnDestroy();
	};
}