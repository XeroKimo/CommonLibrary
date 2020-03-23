#include "CommonsLibrary/ECS/ComponentMap.h"
#include "CommonsLibrary/ECS/GameObject.h"

namespace CommonsLibrary
{
    void ComponentMap::CleanUp()
    {
        if (!componentsToDestroy.empty())
        {
            for (const auto& component : componentsToDestroy)
            {
                component->OnDestroy();
            }
            componentsToDestroy.clear();
        }
    }

    ReferencePointer<Component> ComponentMap::AddComponent(const ReferencePointer<GameObject>& gameObject, const std::type_index& key)
    {
        auto& componentVector = m_componentMap[key];
        componentVector.push_back(ComponentRegistry::Create(key, gameObject));
        return componentVector.back();
    }

    ReferencePointer<Component> ComponentMap::GetComponent(const std::type_index& key)
    {
        if (KeyExists(m_componentMap, key))
            return m_componentMap[key][0];
        return nullptr;
    }

    std::vector<ReferencePointer<Component>> ComponentMap::GetComponents(const std::type_index& key)
    {
        if (KeyExists(m_componentMap, key))
            return m_componentMap[key];
        return {};
    }

    ReferencePointer<Component> ComponentMap::RemoveComponent(const ReferencePointer<Component>& component)
    {
        std::type_index key(typeid(*component));
        auto& componentVector = m_componentMap[key];
        auto it = std::find(componentVector.begin(), componentVector.end(), component);
        if (it != componentVector.end())
        {
            componentsToDestroy.push_back(std::move(*it));
            componentVector.erase(it);
            return component;
        }
        return ReferencePointer<Component>();
    }
}