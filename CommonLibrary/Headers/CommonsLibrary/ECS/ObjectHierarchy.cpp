#include "ObjectHierarchy.h"
#include "GameObject.h"

namespace CommonsLibrary
{
    void ObjectHierarchy::Awake()
    {
        for(auto gameObject : m_activeGameObjects)
            gameObject->Awake();

        for(auto gameObject : m_inactiveGameObjects)
            gameObject->Awake();
    }

    void ObjectHierarchy::PreUpdate()
    {
        if(!m_hasPreUpdateFlagsSet)
        {
            ClearNullChildren();
            ClearNullObjects(m_activeGameObjects);
            ClearNullObjects(m_inactiveGameObjects);
        }
        else
        {
            for(auto gameObject : m_activeGameObjects)
            {
                if(gameObject->HasPreUpdateFlagsSet())
                    gameObject->PreUpdate();
            }
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

        if(m_nextParent)
        {
            SetParent(m_nextParent);
            m_nextParent = nullptr;
        }
        for(auto gameObject : m_activeGameObjects)
        {
            if(gameObject->HasPostUpdateFlagsSet())
                gameObject->HasPostUpdateFlagsSet();
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
        auto updateVec = (gameObject->IsActiveInHeirarchy()) ? m_activeGameObjects : m_inactiveGameObjects;
        updateVec.push_back(gameObject.Get());

        m_children.push_back(std::move(gameObject));
    }
    ReferencePointer<GameObject> ObjectHierarchy::RemoveChild(const ReferencePointer<GameObject>& child)
    {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        auto gameObject = std::move(*it);

        return gameObject;
    }
    ReferencePointer<GameObject> ObjectHierarchy::CreateGameObject()
    {
        m_children.push_back(GameObject::Construct());
        m_inactiveGameObjects.push_back(m_children.back().Get());
        return m_children.back();
    }
    void ObjectHierarchy::ClearNullChildren()
    {
        m_children.erase(std::remove(m_children.begin(), m_children.end(), nullptr), m_children.end());
    }
    void ObjectHierarchy::ClearNullObjects(std::vector<GameObject*>& objectVector)
    {
        objectVector.erase(std::remove(objectVector.begin(), objectVector.end(), nullptr), objectVector.end());
    }
}