#include "CommonsLibrary/ECS/ComponentManager.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include <algorithm>

namespace CommonsLibrary
{
    ComponentManager::~ComponentManager()
    {
        for(auto& component : m_components)
        {
            component->OnDestroyed();
        }
    }

    void ComponentManager::Awake()
    {
        for(const auto& component : m_components)
            component->Awake();
    }
    void ComponentManager::Start()
    {
        ClearDestroyedComponents();
        TransferComponents();
    }
    void ComponentManager::Update(float deltaTime)
    {
        for(size_t i = 0; i < m_lastActiveComponentIndex; i++)
        {
            m_components[i]->Update(deltaTime);
        }
    }

    void ComponentManager::CopyComponents(const ReferencePointer<GameObject>& gameObject, const ComponentManager& other)
    {
        for(const auto& component : other.m_components)
        {
            Copy(gameObject, component);
        }
        m_lastActiveComponentIndex = other.m_lastActiveComponentIndex;
    }

    bool ComponentManager::DestroyComponent(const ReferencePointer<Component>& component)
    {
        if(component->m_isDestroyed)
            return false;

        m_destroyedComponentsIndices.push(component->m_componentIndex);
        component->m_isDestroyed = true;

        return true;
    }
    void ComponentManager::SetComponentActive(const ReferencePointer<Component>& component, bool active)
    {
        if(component->m_active == active)
            return;
        if(component->m_activeChanged)
        {
            m_activeChangedIndices.erase(std::find(m_activeChangedIndices.begin(), m_activeChangedIndices.end(), &component->m_componentIndex));
            component->m_activeChanged = false;
        }
        else
        {
            m_activeChangedIndices.push_back(&component->m_componentIndex);
            component->m_activeChanged = true;
        }

        component->m_active = active;
    }

    ReferencePointer<Component> ComponentManager::CreateComponent(const ReferencePointer<GameObject>& gameObject, std::type_index type)
    {
        size_t index = m_components.size();
        m_components.push_back(ComponentRegistry::CreateComponent(type, gameObject));
        m_components.back()->m_componentIndex = index;

        return m_components.back();
    }

    void ComponentManager::ClearDestroyedComponents()
    {
        if(m_destroyedComponentsIndices.empty())
            return;

        size_t finalIndex = 0;
        while(!m_destroyedComponentsIndices.empty())
        {
            finalIndex = m_destroyedComponentsIndices.top();
            m_components[finalIndex]->OnDestroyed();
            m_components.erase(m_components.begin() + finalIndex);
            m_destroyedComponentsIndices.pop();
        }
        RecountIndicesStarting(finalIndex);
    }

    void ComponentManager::TransferComponents()
    {
        if(m_activeChangedIndices.empty())
            return;

        for(auto componentIndex : m_activeChangedIndices)
        {
            SwapComponentActive(*componentIndex);
        }

        m_activeChangedIndices.clear();
    }

    void ComponentManager::SwapComponentActive(size_t index)
    {
        if(m_components[index]->m_active)
        {
            ++m_lastActiveComponentIndex;

            if(index != m_lastActiveComponentIndex)
                SwapComponents(index, m_lastActiveComponentIndex);
            if(!m_components[index]->m_hasStarted)
                m_components[index]->Start();
        }
        else
        {
            if(index != m_lastActiveComponentIndex)
                SwapComponents(index, m_lastActiveComponentIndex);
            --m_lastActiveComponentIndex;
        }
    }

    void ComponentManager::SwapComponents(size_t lh, size_t rh)
    {
        ReferencePointer<Component> temp = std::move(m_components[lh]);
        m_components[lh] = std::move(m_components[rh]);
        m_components[rh] = std::move(temp);

        m_components[rh]->m_componentIndex = rh;
        m_components[lh]->m_componentIndex = lh;
    }

    ReferencePointer<Component> ComponentManager::Copy(const ReferencePointer<GameObject>& gameObject, const ReferencePointer<Component>& component)
    {
        auto copiedComponent = CreateComponent(gameObject, typeid(*component));
        copiedComponent->CopyComponent(component.Get());
        if(copiedComponent->m_active)
            m_activeChangedIndices.push_back(&copiedComponent->m_componentIndex);

        return copiedComponent;
    }

    void ComponentManager::RecountIndicesStarting(size_t startIndex)
    {
        if(startIndex <= m_lastActiveComponentIndex)
        {
            bool active = true;

            while(active)
            {
                active = m_components[startIndex]->m_active;
                m_components[startIndex]->m_componentIndex = startIndex;

                ++startIndex;
            }

            m_lastActiveComponentIndex = startIndex - 1;
        }

        for(size_t i = startIndex; i < m_components.size(); ++i)
        {
            m_components[i]->m_componentIndex = i;
        }
    }
}