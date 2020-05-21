#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/DebugTools/Logger.h"
#include "CommonsLibrary/StdHelpers/VectorHelpers.h"

namespace CommonsLibrary
{
    void GameObject::SetActive(bool active)
    {
        m_hierarchy.GetParent()->m_hierarchy.SetActive(GetReferencePointer(), active);
    }

    void GameObject::RequestParentChange(const ReferenceView<GameObject>& parent)
    {
        if(!m_owningScene)
            return;

        if(!parent)
            m_hierarchy.RequestParentChange(m_owningScene->GetRootGameObject());
        else
            m_hierarchy.RequestParentChange(parent);
    }

    ReferenceView<GameObject> GameObject::GetParent() const
    {
        if(m_owningScene)
        {
            auto parent = m_hierarchy.GetParent();
            if(parent == m_owningScene->GetRootGameObject())
                return nullptr;
        }
        return m_hierarchy.GetParent();
    }

    bool GameObject::SceneLoaded()
    {
        return m_owningScene->m_isLoaded;
    }

    ReferenceView<GameObject> GameObject::GetRootObject()
    {
        return m_owningScene->GetRootGameObject();
    }

    void GameObject::AddCallChangeComponentsState()
    {
        m_owningScene->m_changeComponentState.push_back(GetReferencePointer());
    }

    void GameObject::AddCallTransferParent()
    {
        m_owningScene->m_transferParents.push_back(GetReferencePointer());
    }

    void GameObject::AddCallChangeChildrenState()
    {
        m_owningScene->m_changeChildrenState.push_back(GetReferencePointer());
    }

    void DestroyGameObject(const ReferencePointer<GameObject>& gameObject)
    {
        gameObject->GetParent()->m_hierarchy.DestroyGameObject(gameObject);
    }


    void DestroyGameObject(const ReferencePointer<Component>& component)
    {
        DestroyGameObject(component->GetGameObject());
    }

    void DestroyComponent(const ReferencePointer<Component>& component)
    {
        if(typeid(*component) == typeid(Transform))
            return;
        component->GetGameObject()->DestroyComponent(component);
    }

}
