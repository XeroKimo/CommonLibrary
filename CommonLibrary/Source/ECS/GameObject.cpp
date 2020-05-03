#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/DebugTools/Logger.h"
#include "CommonsLibrary/StdHelpers/VectorHelpers.h"

namespace CommonsLibrary
{
    void GameObject::PreAwake()
    {
        m_hierarchy.PreAwake();
    }

    void GameObject::SetActive(bool active)
    {
        m_hierarchy.GetParent()->m_hierarchy.SetActive(GetReferencePointer(), active);
    }

    void GameObject::SetParent(const ReferencePointer<GameObject>& parent)
    {
        if(!m_owningScene)
            return;

        if(!parent)
            m_hierarchy.RequestParentChange(m_owningScene->GetRootGameObject());
        else
            m_hierarchy.RequestParentChange(parent);
    }

    ReferencePointer<GameObject> GameObject::GetParent() const
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

    ReferencePointer<GameObject> GameObject::GetRootObject()
    {
        return m_owningScene->GetRootGameObject();
    }

    void GameObject::AddCallStartOnComponents()
    {
        m_owningScene->m_componentStarts.push_back(GetReferencePointer());
    }

    void GameObject::AddCallStartOnHierarchy()
    {
        m_owningScene->m_hierarchyStarts.push_back(GetReferencePointer());
    }

    void GameObject::AddPostStartCall()
    {
        m_owningScene->m_postStartCalls.push_back(GetReferencePointer());
    }

    //ReferencePointer<GameObject> GameObject::Construct()
    //{
    //    ReferencePointer<GameObject> newObject = ReferencePointer<GameObject>(new GameObject());
    //    return newObject;
    //}

    //ReferencePointer<GameObject> GameObject::CopyConstruct(const ReferencePointer<GameObject>& other)
    //{
    //    ReferencePointer<GameObject> newObject = ReferencePointer<GameObject>(new GameObject());

    //    newObject->name = other->name;
    //    newObject->CopyComponents(other->m_componentManager);

    //    return newObject;
    //}



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
