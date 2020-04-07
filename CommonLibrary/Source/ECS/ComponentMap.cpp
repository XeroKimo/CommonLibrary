#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/ComponentMap.h"

namespace CommonsLibrary
{
    void ComponentMap::CleanUp()
    {
        if (!m_componentsToDestroy.empty())
        {
            for (const auto& component : m_componentsToDestroy)
            {
                component->OnDestroy();
            }
            m_componentsToDestroy.clear();
        }
    }

    void ComponentMap::Start()
    {
        for (auto component : m_updateComponents)
            component->StartComponent();
    }

    void ComponentMap::Update(float deltaTime)
    {
        for (auto component : m_updateComponents)
            component->UpdateComponent(deltaTime);
    }

    void ComponentMap::OnDestroy()
    {
        if (!m_componentsToDestroy.empty())
        {
            for (auto component : m_componentsToDestroy)
                component->DestroyComponent();
            m_componentsToDestroy.clear();
        }
    }

    ReferencePointer<Component> ComponentMap::AddComponent(const ReferencePointer<GameObject>& gameObject, const std::type_index& key)
    {
        auto& componentVector = m_componentMap[key];
        componentVector.push_back(ComponentRegistry::Create(key, gameObject));
        m_updateComponents.push_back(componentVector.back().Get());
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

    void ComponentMap::RemoveComponent(const ReferencePointer<Component>& component)
    {
        std::type_index key(typeid(*component));
        auto& componentVector = m_componentMap[key];
        auto it = std::find(componentVector.begin(), componentVector.end(), component);
        if (it != componentVector.end())
        {
            m_componentsToDestroy.push_back(std::move(*it));
            componentVector.erase(it);
        }
    }
    void ComponentMap::DestroyAllComponents()
    {
        for (auto componentVector : m_componentMap)
        {
            for (auto component : componentVector.second)
            {
                RemoveComponent(component);
            }
        }
    }
}