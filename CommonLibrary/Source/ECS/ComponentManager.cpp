#include "CommonsLibrary/ECS/ComponentManager.h"
#include "CommonsLibrary/ECS/GameObject.h"
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
        TransferComponents();

        for(const auto& component : m_activeComponents)
            component->Awake();
        for(const auto& component : m_inactiveComponents)
            component->Awake();
    }

    void ComponentManager::TransferComponents()
    {
        if(m_hasActiveChangedToInactive > 0)
        {
            m_hasActiveChangedToInactive = 0;
            TransferComponents(m_activeComponents, m_inactiveComponents);
        }
        if(m_hasActiveChangedToActive > 0)
        {
            m_hasActiveChangedToActive = 0;
            TransferComponents(m_inactiveComponents, m_activeComponents);
        }
    }
    void ComponentManager::Start()
    {
        if(!m_hasStartComponents)
            return;

        m_hasStartComponents = false;

        for(auto& component : m_activeComponents)
        {
            if(component->m_hasStarted)
                continue;

            component->Start();
            component->m_hasStarted = true;
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
        if(m_hasDestroyInActive > 0)
        {
            m_hasDestroyInActive = 0;
            DestroyComponents(m_activeComponents);
        }
        if(m_hasDestroyInInactive > 0)
        {
            m_hasDestroyInInactive = 0;
            DestroyComponents(m_inactiveComponents);
        }
    }

    ReferencePointer<Component> ComponentManager::CreateComponent(const ReferencePointer<GameObject>& gameObject, bool callAwake, std::type_index type)
    {
        m_hasStartComponents = true;
        m_hasActiveChangedToActive++;

        m_inactiveComponents.push_back(ComponentRegistry::CreateComponent(type, gameObject));
        if(callAwake)
            m_inactiveComponents.back()->Awake();

        return m_inactiveComponents.back();
    }

    bool ComponentManager::DestroyComponent(const ReferencePointer<Component>& component)
    {
        if(component->m_isDestroyed)
            return false;

        if(component->m_activeChanged)
        {
            if(component->m_active)
                m_hasActiveChangedToActive--;
            else
                m_hasActiveChangedToInactive--;

            component->m_active = !component->m_active;
        }

        if(component->m_active)
            m_hasDestroyInActive++;
        else
            m_hasDestroyInInactive++;


        component->m_isDestroyed = true;

        return true;
    }
    void ComponentManager::SetComponentActive(const ReferencePointer<Component>& component, bool active)
    {
        if(component->m_active == active)
            return;

        if(component->m_activeChanged)
        {
            if(!active)
                m_hasActiveChangedToActive--;
            else
                m_hasActiveChangedToInactive--;

            component->m_activeChanged = false;
        }
        else
        {
            if(active)
                m_hasActiveChangedToActive++;
            else
                m_hasActiveChangedToInactive++;

            component->m_activeChanged = true;
        }

        component->m_active = active;
    }

    void ComponentManager::CopyComponents(const ReferencePointer<GameObject>& gameObject, const ComponentManager& other)
    {
        for(const auto& component : other.m_activeComponents)
        {
            Copy(gameObject, component);
        }
        for(const auto& component : other.m_inactiveComponents)
        {
            Copy(gameObject, component);
        }
    }

    void ComponentManager::CallDestroyAll(std::vector<ReferencePointer<Component>>& componentVector)
    {
        for(auto& component : componentVector)
        {
            component->OnDestroyed();
        }
    }

    void ComponentManager::TransferComponents(ComponentVector& from, ComponentVector& to)
    {
        for(auto& component : from)
        {
            if(component->m_isDestroyed)
                continue;

            if(!component->m_activeChanged)
                continue;

            if(component->m_active)
            {
                if(!component->m_hasStarted)
                {
                    m_hasStartComponents = true;
                }
            }

            component->m_activeChanged = false;

            to.push_back(std::move(component));
        }

        ClearNullComponents(from);
    }

    void ComponentManager::DestroyComponents(ComponentVector& components)
    {
        for(auto& component : components)
        {
            if(!component->m_isDestroyed)
                continue;

            component->OnDestroyed();
            component = nullptr;
        }

        ClearNullComponents(components);
    }

    void ComponentManager::ClearNullComponents(ComponentVector& componentVector)
    {
        auto it = std::remove(componentVector.begin(), componentVector.end(), nullptr);
        if(it != componentVector.end())
            componentVector.erase(it, componentVector.end());
    }

    ReferencePointer<Component> ComponentManager::Copy(const ReferencePointer<GameObject>& gameObject, const ReferencePointer<Component>& component)
    {
        ReferencePointer<Component> copy = CreateComponent(gameObject, false, typeid(*component));

        *m_inactiveComponents.back() = *component;
        m_inactiveComponents.back()->m_gameObject = gameObject;

        return m_inactiveComponents.back();
    }
}