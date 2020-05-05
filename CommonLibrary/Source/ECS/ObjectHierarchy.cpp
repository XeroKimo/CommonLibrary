#include "CommonsLibrary/ECS/ObjectHierarchy.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include <assert.h>
namespace CommonsLibrary
{
    void ObjectHierarchy::Awake()
    {
        for(auto child : m_children)
            child->Awake();
    }

    void ObjectHierarchy::TransferParent()
    {
        m_toldSceneToTransferParent = false;
        ChangeParent();
    }
    void ObjectHierarchy::ChangeChildrenState()
    {
        m_toldSceneToChangeChildrenState = false;

        ClearDestroyedChildren();
        SwapChildrenStates();
    }
    void ObjectHierarchy::Update(float deltaTime)
    {
        for(size_t i = 0; i < m_firstInactiveObjectIndex; i++)
        {
            m_children[i]->Update(deltaTime);
        }
    }


    void ObjectHierarchy::RequestParentChange(const ReferencePointer<GameObject>& parent)
    {
        if(parent == m_parent)
            m_nextParent = nullptr;
        else
            m_nextParent = parent;

        AddToStartCall();
    }
    void ObjectHierarchy::SetParent(const ReferencePointer<GameObject>& parent)
    {
        ReferencePointer<GameObject> owningSelf = m_parent->m_hierarchy.RemoveChild(m_gameObject->GetReferencePointer());
        if(m_gameObject->m_isDestroyed)
            return;

        m_parent = parent;
        m_parent->m_hierarchy.AddChild(std::move(owningSelf));
    }
    void ObjectHierarchy::AddChild(ReferencePointer<GameObject> child)
    {
        child->m_childIndex = m_children.size();
        child->m_hasRequestedActiveChanged = false;
        if(child->m_activeChanged && child->m_active)
            m_activeChangedObjects.push_back(child);
        else
            child->m_activeChanged = false;

        m_children.push_back(std::move(child));
    }
    ReferencePointer<GameObject> ObjectHierarchy::RemoveChild(const ReferencePointer<GameObject>& child)
    {
        auto it = (m_children.begin() + child->m_childIndex);
        auto gameObject = std::move(*it);

        m_destroyedObjectIndices.push(child->m_childIndex);

        AddToPostStartCall();

        return gameObject;
    }


    void ObjectHierarchy::SetActive(const ReferencePointer<GameObject>& child, bool active)
    {
        if(child->m_active == active)
            return;

        if(child->m_activeChanged)
        {
            child->m_activeChanged = false;
        }
        else
        {
            if(!child->m_hasRequestedActiveChanged)
            {
                m_activeChangedObjects.push_back(child);
                child->m_hasRequestedActiveChanged;
            }
            child->m_activeChanged = true;
        }

        child->m_active = active;

        AddToPostStartCall();
    }
    void ObjectHierarchy::DestroyGameObject(const ReferencePointer<GameObject>& child)
    {
        if(child->m_isDestroyed)
            return;

        m_destroyedObjectIndices.push(child->m_childIndex);
        child->m_isDestroyed = true;

        AddToPostStartCall();
    }
    ReferencePointer<GameObject> ObjectHierarchy::CreateGameObject()
    {
        //if(sceneLoading)
        //{
        //    m_children.insert(m_children.begin() + m_firstInactiveObjectIndex, new GameObject());
        //    m_children[m_firstInactiveObjectIndex]->m_childIndex = m_firstInactiveObjectIndex;
        //    m_children[m_firstInactiveObjectIndex]->m_hierarchy.m_parent = m_gameObject->GetReferencePointer();
        //    m_children[m_firstInactiveObjectIndex]->m_owningScene = m_gameObject->m_owningScene;
        //    m_firstInactiveObjectIndex++;

        //    return m_children[m_firstInactiveObjectIndex - 1];
        //}
        //else
        {
            size_t index = m_children.size();
            m_children.push_back(new GameObject());
            m_children.back()->m_childIndex = index;
            m_children.back()->m_hierarchy.m_parent = m_gameObject->GetReferencePointer();
            m_children.back()->m_owningScene = m_gameObject->m_owningScene;

            m_activeChangedObjects.push_back(m_children.back());

            AddToPostStartCall();

            return m_children.back();
        }
    }

    void ObjectHierarchy::AddToStartCall()
    {
        if(!m_toldSceneToTransferParent)
        {
            m_toldSceneToTransferParent = true;
            m_gameObject->AddCallTransferParent();
        }
    }
    void ObjectHierarchy::AddToPostStartCall()
    {
        if(!m_toldSceneToChangeChildrenState)
        {
            m_toldSceneToChangeChildrenState = true;
            m_gameObject->AddCallChangeChildrenState();
        }
    }

    void ObjectHierarchy::ClearDestroyedChildren()
    {
        if(m_destroyedObjectIndices.empty())
            return;

        size_t finalIndex = 0;
        while(!m_destroyedObjectIndices.empty())
        {
            finalIndex = m_destroyedObjectIndices.top();
            m_children.erase(m_children.begin() + finalIndex);
            m_destroyedObjectIndices.pop();
        }

        RecountIndicesStarting(finalIndex);
    }
    void ObjectHierarchy::SwapChildrenStates()
    {
        if(m_activeChangedObjects.empty())
            return;

        for(auto child : m_activeChangedObjects)
        {
            if(!child)
                continue;
            if(child->GetParent().Get() != m_gameObject)
                continue;

            SwapObjectActive(child->m_childIndex);
        }

        m_activeChangedObjects.clear();
    }
    void ObjectHierarchy::ChangeParent()
    {
        if(!m_nextParent)
            return;

        SetParent(m_nextParent);
        m_nextParent = nullptr;
    }

    void ObjectHierarchy::SwapObjectActive(size_t objectIndex)
    {
         m_children[objectIndex]->m_hasRequestedActiveChanged = false;

        if(!m_children[objectIndex]->m_activeChanged)
            return;

        m_children[objectIndex]->m_activeChanged = false;

        if(m_children[objectIndex]->m_active)
        {
            if(objectIndex != m_firstInactiveObjectIndex)
                SwapObject(objectIndex, m_firstInactiveObjectIndex);

            if(m_children[m_firstInactiveObjectIndex]->m_componentManager.HasStartFlagsSet())
            {
                m_children[m_firstInactiveObjectIndex]->m_componentManager.AddToStartCall();
            }

            ++m_firstInactiveObjectIndex;
        }
        else
        {
            --m_firstInactiveObjectIndex;
            if(objectIndex != m_firstInactiveObjectIndex)
                SwapObject(objectIndex, m_firstInactiveObjectIndex);
        }

    }
    void ObjectHierarchy::SwapObject(size_t lh, size_t rh)
    {
        ReferencePointer<GameObject> temp = std::move(m_children[lh]);
        m_children[lh] = std::move(m_children[rh]);
        m_children[rh] = std::move(temp);

        m_children[rh]->m_childIndex = rh;
        m_children[lh]->m_childIndex = lh;
    }

    void ObjectHierarchy::RecountIndicesStarting(size_t startIndex)
    {
        if(startIndex == m_children.size())
            return;

        if(startIndex < m_firstInactiveObjectIndex)
        {
            bool active = true;

            while(active && startIndex < m_children.size())
            {
                active = m_children[startIndex]->m_active;
                m_children[startIndex]->m_childIndex = startIndex;

                ++startIndex;
            }

            m_firstInactiveObjectIndex = startIndex - 1;
        }

        for(size_t i = startIndex; i < m_children.size(); ++i)
        {
            m_children[i]->m_childIndex = i;
        }
    }
}