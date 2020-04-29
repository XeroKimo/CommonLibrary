#include "CommonsLibrary/ECS/ComponentManager.h"
#include <algorithm>

namespace CommonsLibrary
{
    ComponentManager::~ComponentManager()
    {
        CallDestroyAll(m_activeComponents);
        CallDestroyAll(m_inactiveComponents);
    }

    void ComponentManager::Awake()
    {
        for(const auto& component : m_activeComponents)
        {
            component->Awake();
        }
        for(const auto& component : m_inactiveComponents)
        {
            component->Awake();
        }
    }

    void ComponentManager::TransferComponents()
    {
        if(!m_hasActiveChangedComponents)
            return;
        m_hasActiveChangedComponents = false;

        ComponentVector output = BeginTransferComponents();
        ClearTransferedComponents();
        MoveStartComponents(output);
    }
    void ComponentManager::Start()
    {
        if(!m_hasStartComponents)
            return;
        m_hasStartComponents = false;

        for(auto& component : m_bufferComponents)
        {
            if(!component)
                continue;

            if(!component->m_active)
                continue;
            if(component->m_hasStarted)
                continue;

            component->Start();
            component->m_hasStarted = true;

            if(!component->m_isDestroyed)
                component = nullptr;
        }
    }
    void ComponentManager::Update(float deltaTime)
    {
        for(auto component : m_activeComponents)
        {
            component->Update(deltaTime);
        }
    }
    void ComponentManager::ClearDestroyedComponents()
    {
        if(!m_hasDestroyComponents)
            return;
        m_hasDestroyComponents = false;

        for(auto& component : m_bufferComponents)
        {
            if(!component)
                continue;
            if(component->m_isDestroyed)
                component->OnDestroyed();

            if(component->m_active)
                m_activeComponents.erase(std::find(m_activeComponents.begin(), m_activeComponents.end(), component));
            else
                m_inactiveComponents.erase(std::find(m_activeComponents.begin(), m_activeComponents.end(), component));

            component = nullptr;
        }

    }
    void ComponentManager::ClearFlags()
    {
        if(m_bufferComponents.empty())
            return;

        m_bufferComponents.erase
        (
            std::remove_if(m_bufferComponents.begin(), m_bufferComponents.end(), [](ComponentVector::value_type it) { return it == nullptr; }),
            m_bufferComponents.end()
        );
    }

    ReferencePointer<Component> ComponentManager::CreateComponent(const ReferencePointer<GameObject>& gameObject, bool objectConstructed, bool sceneLoaded, std::type_index type)
    {
        m_hasStartComponents = true;
        m_hasActiveChangedComponents = true;

        m_inactiveComponents.push_back(ComponentRegistry::CreateComponent(type, gameObject));
        if(objectConstructed && sceneLoaded)
            m_inactiveComponents.back()->Awake();
        m_bufferComponents.push_back(m_inactiveComponents.back());

        return m_inactiveComponents.back();
    }

    bool ComponentManager::DestroyComponent(const ReferencePointer<Component>& component)
    {
        if(component->m_isDestroyed)
            return false;

        auto& componentVector = (component->m_active) ? m_activeComponents : m_inactiveComponents;
        auto it = std::find(componentVector.begin(), componentVector.end(), component);

        if(it != componentVector.end())
        {
            m_bufferComponents.push_back(*it);
            m_hasDestroyComponents = true;
            component->m_isDestroyed = true;

            return true;
        }

        return false;
    }
    void ComponentManager::SetComponentActive(const ReferencePointer<Component>& component, bool active)
    {
        if(component->m_active == active)
            return;

        auto inBuffer = std::find_if(m_bufferComponents.begin(), m_bufferComponents.end(), [&component](ReferencePointer<Component> it) { return it == component; });

        if(inBuffer != m_bufferComponents.end())
        {
            if((*inBuffer)->m_isDestroyed)
                return;

            m_bufferComponents.erase(inBuffer);
            component->m_active = active;

            return;
        }

        auto& extractVector = (active) ? m_inactiveComponents : m_activeComponents;
        auto it = std::find(extractVector.begin(), extractVector.end(), component);

        if(it == extractVector.end())
            return;

        m_bufferComponents.push_back(*it);
        m_hasActiveChangedComponents = true;
    }

    void ComponentManager::CopyComponents(const ReferencePointer<GameObject>& gameObject, bool objectConstructed, bool sceneLoaded, const ComponentManager& other)
    {
        for(const auto& component : other.m_activeComponents)
        {
            CreateComponent(gameObject, objectConstructed, sceneLoaded, typeid(*component))->CopyComponent(component.Get());
        }
        for(const auto& component : other.m_inactiveComponents)
        {
            CreateComponent(gameObject, objectConstructed, sceneLoaded, typeid(*component))->CopyComponent(component.Get());
        }
    }

    void ComponentManager::CallDestroyAll(std::vector<ReferencePointer<Component>>& componentVector)
    {
        for(auto& component : componentVector)
        {
            component->OnDestroyed();
        }
    }

    ComponentManager::ComponentVector ComponentManager::BeginTransferComponents()
    {
        ComponentManager::ComponentVector output;

        for(const auto& component : m_bufferComponents)
        {
            if(component->m_isDestroyed)
                continue;

            ComponentVector& fromVector = (component->m_active) ? m_inactiveComponents : m_activeComponents;
            ComponentVector& toVector = (component->m_active) ? m_activeComponents : m_inactiveComponents;

            auto it = std::find(fromVector.begin(), fromVector.end(), component);
            toVector.push_back(std::move(*it));

            if(component->m_active)
            {
                if(!m_activeComponents.back()->m_hasStarted)
                {
                    m_hasStartComponents = true;
                }
            }
        }

        return output;
    }
    void ComponentManager::ClearTransferedComponents()
    {
        ClearNullComponents(m_activeComponents);
        ClearNullComponents(m_inactiveComponents);
    }
    void ComponentManager::MoveStartComponents(ComponentVector& startVector)
    {
        std::move(startVector.begin(), startVector.end(), std::back_inserter(m_bufferComponents));
    }

    void ComponentManager::ClearNullComponents(ComponentVector& componentVector)
    {
        auto it = std::remove(componentVector.begin(), componentVector.end(), nullptr);
        if(it != componentVector.end())
            componentVector.erase(it, componentVector.end());
    }
}