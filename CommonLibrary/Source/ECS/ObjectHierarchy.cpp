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

        //TransferObjects(m_inactiveGameObjects, m_activeGameObjects);

    }
    void ObjectHierarchy::Awake()
    {
        for(auto gameObject : m_children)
            gameObject->Awake();
    }

    void ObjectHierarchy::PreUpdate()
    {
        //if(m_hasPreUpdateFlagsSet)
        //{
        //    m_hasPreUpdateFlagsSet = false;

        //    if(m_countOfChangedToActive > 0)
        //    {
        //        m_countOfChangedToActive = 0;
        //        TransferObjects(m_inactiveGameObjects, m_activeGameObjects);
        //    }
        //    if(m_countOfChangedToInactive > 0)
        //    {
        //        m_countOfChangedToInactive = 0;
        //        TransferObjects(m_activeGameObjects, m_inactiveGameObjects);
        //    }
        //}

        //if(m_childHasPreFlagSet)
        //{
        //    m_childHasPostFlagSet = false;
        //    for(auto gameObject : m_children)
        //    {
        //        gameObject->PreUpdate();
        //    }
        //}
    }

    void ObjectHierarchy::Start()
    {
        for(size_t i = 0; i <= m_lastActiveChildIndex; i++)
        {
            m_children[i]->Start();
        }
    }

    void ObjectHierarchy::Update(float deltaTime)
    {
        for(size_t i = 0; i <= m_lastActiveChildIndex; i++)
        {
            m_children[i]->Update(deltaTime);
        }
    }

    void ObjectHierarchy::PostUpdate()
    {
        //if(m_hasPostUpdateFlagsSet)
        //{
        //    m_hasPostUpdateFlagsSet = false;

        //    if(m_nextParent)
        //    {
        //        SetParent(m_nextParent);
        //        m_nextParent = nullptr;
        //    }

        //    if(m_countOfDestroyedObjects > 0)
        //    {
        //        m_countOfDestroyedObjects = 0;

        //        ReferencePointer<GameObject> gameObject;

        //        for(auto it = m_children.begin(); it != m_children.end();)
        //        {
        //            gameObject = (*it);

        //            if(!gameObject->m_isDestroyed)
        //                it++;
        //            else
        //            {
        //                auto& updateVec = (gameObject->IsActiveInHeirarchy()) ? m_activeGameObjects : m_inactiveGameObjects;
        //                updateVec.erase(std::find(updateVec.begin(), updateVec.end(), gameObject.Get()));
        //                it = m_children.erase(it);
        //            }
        //        }
        //    }
        //}

        //if(m_childHasPostFlagSet)
        //{
        //    m_childHasPostFlagSet = false;
        //    auto childrenCopy = m_children;
        //    for(auto gameObject : childrenCopy)
        //    {
        //        gameObject->PostUpdate();
        //    }
        //}
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
        if(gameObject->IsActiveInHeirarchy())
        {
            gameObject->m_childIndex = ++m_lastActiveChildIndex;
            m_children.insert(m_children.begin() + m_lastActiveChildIndex, std::move(gameObject));
            IncreaseIndicesAfter(m_lastActiveChildIndex, 1);
        }
        else
        {
            gameObject->m_childIndex = m_children.size();
            m_children.push_back(std::move(gameObject));
        }
    }
    ReferencePointer<GameObject> ObjectHierarchy::RemoveChild(const ReferencePointer<GameObject>& child)
    {
        ReduceIndicesAfter(child->m_childIndex, 1);

        auto it = (m_children.begin() + child->m_childIndex);
        auto gameObject = std::move(*it);
        m_children.erase(it);

        return gameObject;
    }

    void ObjectHierarchy::SetActive(const ReferencePointer<GameObject>& child, bool active)
    {
        if(child->m_active == active)
            return;

        if(child->m_activeChanged)
        {
            if(!active)
                m_countOfChangedToActive--;
            else
                m_countOfChangedToInactive--;

            child->m_activeChanged = false;
        }
        else
        {
            if(active)
                m_countOfChangedToActive++;
            else
                m_countOfChangedToInactive++;

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
                m_countOfChangedToActive--;
            else
                m_countOfChangedToInactive--;
        }

        m_countOfDestroyedObjects++;

        child->m_isDestroyed = true;

        return;
    }
    ReferencePointer<GameObject> ObjectHierarchy::CreateGameObject()
    {
        m_childrenBuffer.push_back(GameObject::Construct());
        m_childrenBuffer.back()->m_hierarchy.m_parent = m_gameObject->GetReferencePointer();

        m_countOfChangedToActive++;
        SetPreUpdateFlag();

        return m_children.back();
    }
    void ObjectHierarchy::SetPreUpdateFlag()
    {
        m_hasPreUpdateFlagsSet = true;

        if(m_parent)
            m_parent->m_hierarchy.SetParentPreUpdateFlag(true);
    }
    void ObjectHierarchy::SetPostUpdateFlag()
    {
        m_hasPostUpdateFlagsSet = true;
        if(m_parent)
            m_parent->m_hierarchy.SetParentPostUpdateFlag(true);
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
        if(m_childHasPreFlagSet)
            return;

        if(m_parent)
            m_parent->m_hierarchy.SetParentPreUpdateFlag(true);
        m_childHasPreFlagSet = set;
    }
    void ObjectHierarchy::SetParentPostUpdateFlag(bool set)
    {
        if(m_childHasPostFlagSet)
            return;

        if(m_parent)
            m_parent->m_hierarchy.SetParentPostUpdateFlag(true);
        m_childHasPostFlagSet = set;
    }
    void ObjectHierarchy::IncreaseIndicesAfter(size_t index, size_t amount)
    {
        index++;
        for(; index < m_children.size(); index++)
            m_children[index]->m_childIndex += amount;
    }
    void ObjectHierarchy::ReduceIndicesAfter(size_t index, size_t amount)
    {
        index++;
        for(; index < m_children.size(); index++)
            m_children[index]->m_childIndex -= amount;
    }
}