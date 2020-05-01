#include "CommonsLibrary/ECS/ObjectHierarchy.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include <assert.h>
namespace CommonsLibrary
{
    void ObjectHierarchy::PreAwake()
    {
        auto childrenCopy = m_children;
        for(auto gameObject : childrenCopy)
            gameObject->PreAwake();

        if(m_nextParent)
        {
            m_gameObject->m_active = false;
            SetParent(m_nextParent);
            m_nextParent = nullptr;
            m_gameObject->m_active = true;
        }

        TransferObjects(m_inactiveGameObjects, m_activeGameObjects);

    }
    void ObjectHierarchy::Awake()
    {
        for(auto gameObject : m_children)
            gameObject->Awake();
    }

    void ObjectHierarchy::PreUpdate()
    {
        if(!m_hasPreUpdateFlagsSet)
            return;

        m_hasPreUpdateFlagsSet = false;

        if(m_hasActiveChangedToActive > 0)
        {
            m_hasActiveChangedToActive = 0;
            TransferObjects(m_inactiveGameObjects, m_activeGameObjects);
        }
        if(m_hasActiveChangedToInactive > 0)
        {
            m_hasActiveChangedToInactive = 0;
            TransferObjects(m_activeGameObjects, m_inactiveGameObjects);
        }

        for(auto gameObject : m_children)
        {
            gameObject->PreUpdate();
        }
    }

    void ObjectHierarchy::Update(float deltaTime)
    {
        for(auto gameObject : m_activeGameObjects)
        {
            gameObject->Update(deltaTime);
        }
    }

    void ObjectHierarchy::PostUpdate()
    {
        if(!m_hasPostUpdateFlagsSet)
            return;

        m_hasPostUpdateFlagsSet = false;

        if(m_nextParent)
        {
            SetParent(m_nextParent);
            m_nextParent = nullptr;
        }

        if(m_hasDestroyedObjects > 0)
        {
            m_hasDestroyedObjects = 0;

            ReferencePointer<GameObject> gameObject;

            for(auto it = m_children.begin(); it != m_children.end();)
            {
                gameObject = (*it);

                if(!gameObject->m_isDestroyed)
                    it++;
                else
                {
                    auto& updateVec = (gameObject->IsActiveInHeirarchy()) ? m_activeGameObjects : m_inactiveGameObjects;
                    updateVec.erase(std::find(updateVec.begin(), updateVec.end(), gameObject.Get()));
                    it = m_children.erase(it);
                }
            }
        }

        auto childrenCopy = m_children;
        for(auto gameObject : childrenCopy)
        {
            gameObject->PostUpdate();
        }
    }

    void ObjectHierarchy::RequestParentChange(const ReferencePointer<GameObject>& parent)
    {
        if(parent == m_parent)
            m_nextParent = nullptr;
        else
            m_nextParent = parent;
    }

    void ObjectHierarchy::SetParent(const ReferencePointer<GameObject>& parent)
    {
        ReferencePointer<GameObject> owningSelf = m_parent->RemoveChild(m_gameObject->GetReferencePointer());
        m_parent = parent;
        m_parent->AddChild(std::move(owningSelf));
    }
    void ObjectHierarchy::AddChild(ReferencePointer<GameObject> gameObject)
    {
        auto& updateVec = (gameObject->IsActiveInHeirarchy()) ? m_activeGameObjects : m_inactiveGameObjects;
        updateVec.push_back(gameObject.Get());

        m_children.push_back(std::move(gameObject));
    }
    ReferencePointer<GameObject> ObjectHierarchy::RemoveChild(const ReferencePointer<GameObject>& child)
    {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        auto gameObject = std::move(*it);
        m_children.erase(it);

        auto& updateVec = (gameObject->IsActiveInHeirarchy()) ? m_activeGameObjects : m_inactiveGameObjects;
        updateVec.erase(std::find(updateVec.begin(), updateVec.end(), gameObject.Get()));

        return gameObject;
    }

    void ObjectHierarchy::SetActive(const ReferencePointer<GameObject>& child, bool active)
    {
        if(child->m_active == active)
            return;

        if(child->m_activeChanged)
        {
            if(!active)
                m_hasActiveChangedToActive--;
            else
                m_hasActiveChangedToInactive--;

            child->m_activeChanged = false;
        }
        else
        {
            if(active)
                m_hasActiveChangedToActive++;
            else
                m_hasActiveChangedToInactive++;

            child->m_activeChanged = true;
        }

        child->m_active = active;
    }
    void ObjectHierarchy::DestroyGameObject(const ReferencePointer<GameObject>& child)
    {
        if(child->m_isDestroyed)
            return;

        if(child->m_activeChanged)
        {
            if(child->m_active)
                m_hasActiveChangedToActive--;
            else
                m_hasActiveChangedToInactive--;
        }

        m_hasDestroyedObjects++;

        child->m_isDestroyed = true;

        return;
    }
    ReferencePointer<GameObject> ObjectHierarchy::CreateGameObject()
    {
        m_children.push_back(GameObject::Construct());
        m_inactiveGameObjects.push_back(m_children.back().Get());
        m_inactiveGameObjects.back()->m_hierarchy.m_parent = m_gameObject->GetReferencePointer();

        m_hasActiveChangedToActive++;
        SetPreUpdateFlag();

        return m_children.back();
    }
    void ObjectHierarchy::SetPreUpdateFlag()
    {
        SetParentPreUpdateFlag(true);
    }
    void ObjectHierarchy::SetPostUpdateFlag()
    {
        SetParentPostUpdateFlag(true);
    }
    void ObjectHierarchy::TransferObjects(std::vector<GameObject*>& from, std::vector<GameObject*>& to)
    {
        GameObject* gameObject;

        for(auto it = from.begin(); it != from.end();)
        {
            gameObject = (*it);

            if(gameObject->m_isDestroyed || !gameObject->m_activeChanged)
                it++;
            else
            {
                gameObject->m_activeChanged = false;
                to.push_back(gameObject);
                it = from.erase(it);
            }
        }
    }
    void ObjectHierarchy::SetParentPreUpdateFlag(bool set)
    {
        if(m_hasPreUpdateFlagsSet)
            return;

        if(m_parent)
            m_parent->m_hierarchy.SetParentPreUpdateFlag(true);
        m_hasPreUpdateFlagsSet = set;
    }
    void ObjectHierarchy::SetParentPostUpdateFlag(bool set)
    {
        if(m_hasPostUpdateFlagsSet)
            return;

        if(m_parent)
            m_parent->m_hierarchy.SetParentPostUpdateFlag(true);
        m_hasPostUpdateFlagsSet = set;
    }
}