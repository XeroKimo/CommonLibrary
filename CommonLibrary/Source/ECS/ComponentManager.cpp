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
        for(size_t i = 0; i < m_components.size(); i++)
            m_components[i]->Awake();
    }
    void ComponentManager::Start()
    {
        m_toldSceneToCallStart = false;
        if(m_gameObject->IsActiveInHeirarchy())
        {
            ClearDestroyedComponents();
            TransferComponents();
        }
    }
    void ComponentManager::Update(float deltaTime)
    {
        for(size_t i = 0; i < m_firstInactiveComponentIndex; i++)
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
        m_firstInactiveComponentIndex = other.m_firstInactiveComponentIndex;
    }

    bool ComponentManager::DestroyComponent(const ReferencePointer<Component>& component)
    {
        if(component->m_isDestroyed)
            return false;

        m_destroyedComponentsIndices.push(component->m_componentIndex);
        component->m_isDestroyed = true;

        AddToStartCall();

        return true;
    }
    void ComponentManager::SetComponentActive(const ReferencePointer<Component>& component, bool active)
    {
        if(component->m_active == active)
            return;
        if(component->m_activeChanged)
        {
            component->m_activeChanged = false;
        }
        else
        {
            if(!component->m_hasRequestedActiveChanged)
            {
                m_activeChangedComponents.push_back(component);
                component->m_hasRequestedActiveChanged = true;
            }
            component->m_activeChanged = true;
        }

        if(!m_toldSceneToCallStart)
        {
            m_toldSceneToCallStart = true;
            m_gameObject->AddCallStartOnComponents();
        }

        AddToStartCall();

        component->m_active = active;
    }

    void ComponentManager::AddToStartCall()
    {
        if(!m_toldSceneToCallStart)
        {
            m_toldSceneToCallStart = true;
            m_gameObject->AddCallStartOnComponents();
        }
    }

    ReferencePointer<Component> ComponentManager::CreateComponent(const ReferencePointer<GameObject>& gameObject, std::type_index type)
    {
        size_t index = m_components.size();
        m_components.push_back(ComponentRegistry::CreateComponent(type, gameObject));
        m_components.back()->m_componentIndex = index;

        AddToStartCall();

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
        if(m_activeChangedComponents.empty())
            return;

        for(auto component : m_activeChangedComponents)
        {
            if(!component)
                continue;

            SwapComponentActive(component->m_componentIndex);
        }

        m_activeChangedComponents.clear();
    }

    void ComponentManager::SwapComponentActive(size_t index)
    {
        m_components[index]->m_hasRequestedActiveChanged = false;
        if(!m_components[index]->m_activeChanged)
            return;
        m_components[index]->m_activeChanged = false;

        if(m_components[index]->m_active)
        {
            if(index > m_firstInactiveComponentIndex)
                SwapComponents(index, m_firstInactiveComponentIndex);

            if(!m_components[m_firstInactiveComponentIndex]->m_hasStarted)
                m_components[m_firstInactiveComponentIndex]->Start();

            m_firstInactiveComponentIndex++;
        }
        else
        {
            --m_firstInactiveComponentIndex;
            if(index > m_firstInactiveComponentIndex)
                SwapComponents(index, m_firstInactiveComponentIndex);
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
            m_activeChangedComponents.push_back(copiedComponent);

        return copiedComponent;
    }

    void ComponentManager::RecountIndicesStarting(size_t startIndex)
    {
        if(startIndex < m_firstInactiveComponentIndex)
        {
            bool active = true;

            while(active)
            {
                active = m_components[startIndex]->m_active;
                m_components[startIndex]->m_componentIndex = startIndex;

                ++startIndex;
            }

            m_firstInactiveComponentIndex = startIndex - 1;
        }

        for(size_t i = startIndex; i < m_components.size(); ++i)
        {
            m_components[i]->m_componentIndex = i;
        }
    }
}