#include "CommonsLibrary/ECS/GameObjectManager.h"

namespace CommonsLibrary
{
    void GameObjectManager::Awake()
    {
        TransferObjects();
        for(const auto& gameObject : m_activeGameObjects)
        {
            gameObject->Awake();
        }
        for(const auto& gameObject : m_inactiveGameObjects)
        {
            gameObject->Awake();
        }
    }

    void GameObjectManager::TransferObjects()
    {
    }

    void GameObjectManager::PreUpdate()
    {
        for(const auto& gameObject : m_activeGameObjects)
        {
            if(gameObject->HasPreUpdateFlagsSet())
                gameObject->PreUpdate();
        }
    }

    void GameObjectManager::Update(float deltaTime)
    {
        for(const auto& gameObject : m_activeGameObjects)
        {
            gameObject->Update(deltaTime);
        }
    }

    void GameObjectManager::PostUpdate()
    {
        for(auto& gameObject : m_activeGameObjects)
        {
            if(gameObject->HasPostUpdateFlagsSet())
                gameObject->PostUpdate();
        }
        for(auto& gameObject : m_inactiveGameObjects)
        {
            if(gameObject->HasPostUpdateFlagsSet())
                gameObject->PostUpdate();
        }
    }

    void GameObjectManager::SetActiveInHeirarchy(const ReferencePointer<GameObject>& gameObject, bool active)
    {
        if(gameObject->IsActiveInHeirarchy() == active)
            return;

        gameObject->m_isActiveInHeirarchy = active;

        SetActiveInWorld(gameObject, active && gameObject->m_transform->IsHeirarchyActive());
    }
    void GameObjectManager::SetActiveInWorld(const ReferencePointer<GameObject>& gameObject, bool active)
    {
        if(gameObject->IsActiveInWorld() == active)
            return;

        gameObject->m_isActiveWorld = active;

        auto inBuffer = std::find(m_bufferGameObjects.begin(), m_bufferGameObjects.end(), gameObject);

        if(inBuffer == m_bufferGameObjects.end())
        {
            m_bufferGameObjects.push_back(gameObject);
            m_hasActiveChangedComponents = true;
        }
        else
        {
            if(gameObject->m_isDestroyed)
                return;

            m_bufferGameObjects.erase(inBuffer);
        }
    }
}
