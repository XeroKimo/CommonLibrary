#include "ECS/Scene.h"
#include "ECS/Component.h"
#include "VectorHelpers.h"

namespace CommonsLibrary
{
    void Scene::Update(float deltaTime)
    {
        for (const ReferencePointer<GameObject>& gameObject : m_activeGameObjects)
            gameObject->Update(deltaTime);
    }
    void Scene::AddGameObject(ReferencePointer<GameObject> gameObject)
    {
        if (gameObject->IsActiveInWorld())
            m_activeGameObjects.push_back(std::move(gameObject));
        else
            m_inactiveGameObjects.push_back(std::move(gameObject));
    }
    void Scene::DeleteGameObject(const ReferencePointer<GameObject>& gameObject)
    {
        if (!FindObjectToDelete(m_activeGameObjects, gameObject))
            FindObjectToDelete(m_inactiveGameObjects, gameObject);
    }
    void Scene::SetObjectActive(const ReferencePointer<GameObject>& gameObject)
    {
        bool isActive = gameObject->IsActiveInWorld();
        auto& toRemoveVector = (isActive) ? m_inactiveGameObjects : m_activeGameObjects;
        auto& toAddVector = (isActive) ? m_activeGameObjects : m_inactiveGameObjects;

        auto it = std::find(toRemoveVector.begin(), toRemoveVector.end(), gameObject);
        toAddVector.push_back(std::move(*it));
        toRemoveVector.erase(it);
    }
    ReferencePointer<GameObject> Scene::FindObject(const std::string& name)
    {
        ReferencePointer<GameObject> objectToFind = FindObject(m_activeGameObjects, name);
        return (objectToFind) ? objectToFind : FindObject(m_inactiveGameObjects, name);
    }
    bool Scene::FindObjectToDelete(std::vector<ReferencePointer<GameObject>>& objectVector, const ReferencePointer<GameObject>& gameObject)
    {
        return RemoveFromVector(objectVector, gameObject);
    }
    ReferencePointer<GameObject> Scene::FindObject(const std::vector<ReferencePointer<GameObject>>& objectVector, const std::string& name)
    {
        for (const ReferencePointer<GameObject>& gameObject : objectVector)
        {
            if (gameObject->name == name)
                return gameObject;
        }
        return nullptr;
    }
}