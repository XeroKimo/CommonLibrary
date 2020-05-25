#include "CommonsLibrary/ECS/ECS.h"
#include <algorithm>

namespace CommonsLibrary
{
    GameObject::~GameObject()
    {
        if(!m_owningScene->m_sceneDestroyed)
        {
            for(const auto& [key, componentVector] : m_componentMap)
            {
                for(const auto& component : componentVector)
                {
                    component->OnDestroyed();
                    m_owningScene->DestroyComponent(component);
                }
            }
        }
    }

    void GameObject::Awake()
    {
        for(const auto& [key, componentVector] : m_componentMap)
        {
            for(const auto& component : componentVector)
                component->Awake();
        }
    }

    ReferenceView<Component> GameObject::AddComponent(std::type_index type)
    {
        std::string typeName = type.name();
        ReferenceView<Component> component = m_owningScene->CreateComponent(typeName, GetReferencePointer());
        m_componentMap[typeName].push_back(component);

        if(m_owningScene->IsSceneLoaded() && m_finishedConstruction)
            component->Awake();

        return component;
    }

    void GameObject::SetParent(const ReferenceView<GameObject>& gameObject)
    {
        bool previousActiveHeiarchy = IsActiveInHeirarchy();

        if(m_parent)
            m_parent->RemoveChild(this);

        m_parent = gameObject.Get();

        if(m_parent)
            m_parent->AddChild(this);

        bool currentActiveHeirarchy = IsActiveInHeirarchy();

        if(currentActiveHeirarchy != previousActiveHeiarchy)
            SetWorldActive(currentActiveHeirarchy);
    }
    void GameObject::SetActive(bool active)
    {
        if(active == m_active)
            return;

        bool previousActiveHeiarchy = IsActiveInHeirarchy();

        active = m_active;

        bool currentActiveHeirarchy = IsActiveInHeirarchy();

        if(previousActiveHeiarchy != currentActiveHeirarchy)
            SetWorldActive(currentActiveHeirarchy);
    }

    void GameObject::RemoveComponent(const ReferenceView<Component>& component)
    {
        std::vector<ReferenceView<Component>>& componentVector = m_componentMap[typeid(*component).name()];

        componentVector.erase(std::remove_if(componentVector.begin(), componentVector.end(), [component](const auto& compare) { return compare == component; }));

        component->OnDestroyed();

        m_owningScene->DestroyComponent(component);
    }

    void GameObject::SetComponentActive(const ReferenceView<Component>& component, bool active)
    {
        m_owningScene->SetComponentActive(component, active);
    }

    void GameObject::AddChild(GameObject* gameObject)
    {
        gameObject->m_childIndex = m_children.size();
        m_children.push_back(gameObject);
    }
    void GameObject::RemoveChild(GameObject* gameObject)
    {
        m_children.back()->m_childIndex = gameObject->m_childIndex;
        m_children[gameObject->m_childIndex] = m_children.back();
        m_children.pop_back();
    }
    void GameObject::SetWorldActive(bool active)
    {
        m_owningScene->SetGameObjectActive(GetReferencePointer(), active);
    }
    void GameObject::SetAllComponentsWorldActive(bool active)
    {
        for(const auto& [key, componentVector] : m_componentMap)
        {
            for(const auto& component : componentVector)
            {
                m_owningScene->SetComponentActiveWorld(component, active);
            }
        }
    }
    void GameObject::SetChildrenWorldActive(bool active)
    {
        for(auto child : m_children)
        {
           if(!child->m_active && !active)
               continue;

           SetAllComponentsWorldActive(active);
           child->SetChildrenWorldActive(active);
        }
    }
}
