#include "CommonsLibrary/ECS/GameObjectManager.h"

namespace CommonsLibrary
{
    void GameObjectManager::Awake()
    {
        for(const auto& gameObject : m_activeGameObjects)
        {
            gameObject->Awake();
        }
        for(const auto& gameObject : m_inactiveGameObjects)
        {
            gameObject->Awake();
        }
    }

    void GameObjectManager::PreUpdate()
    {
        for(const auto& gameObject : m_bufferGameObjects)
        {
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
        for(auto& gameObject : m_bufferGameObjects)
        {
            gameObject->PostUpdate();
        }
    }

    void GameObjectManager::ClearBuffer()
    {
        if(m_bufferGameObjects.empty())
            return;

        m_bufferGameObjects.erase
        (
            std::remove_if(m_bufferGameObjects.begin(), m_bufferGameObjects.end(), [](GameObjectVector::value_type it) { return it == nullptr; }),
            m_bufferGameObjects.end()
        );
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
