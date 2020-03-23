#pragma once
#include "Component.h"
#include "Transform.h"
#include <typeindex>

namespace CommonsLibrary
{
	__interface IGameObject
	{
		ReferencePointer<Component> AddComponent(const std::type_index& key);
		void RemoveComponent(const ReferencePointer<Component>& component);

		ReferencePointer<Component> GetComponent(const std::type_index& key);
		std::vector<ReferencePointer<Component>> GetComponents(const std::type_index& key);

		ReferencePointer<Transform> GetTransform();

		void SetName(std::string_view name);
		std::string GetName() const;

		void SetActive(bool active);
		bool GetActiveHeirarchy() const;
		bool GetActiveWorld() const;

		void Destroy();
	};
}