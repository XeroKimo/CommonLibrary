#include "ECS/Scene.h"
#include "ECS/Component.h"

namespace CommonsLibrary
{
    void Scene::Update(float deltaTime)
    {
        for (const ReferencePointer<GameObject>& gameObject : m_activeGameObjects)
            gameObject->Update(deltaTime);
    }
    void Scene::AddGameObject(ReferencePointer<GameObject> gameObject)
    {
        m_activeGameObjects.push_back(std::move(gameObject));
    }
    void Scene::DeleteGameObject(const ReferencePointer<GameObject>& gameObject)
    {
        if (!FindObjectToDelete(m_activeGameObjects, gameObject))
            FindObjectToDelete(m_inactiveGameObjects, gameObject);
    }
    ReferencePointer<GameObject> Scene::FindObject(const std::string& name)
    {
        ReferencePointer<GameObject> objectToFind = FindObject(m_activeGameObjects, name);
        return (objectToFind) ? objectToFind : FindObject(m_inactiveGameObjects, name);
    }
    bool Scene::FindObjectToDelete(std::vector<ReferencePointer<GameObject>>& objectVector, const ReferencePointer<GameObject>& gameObject)
    {
        auto it = std::find(objectVector.begin(), objectVector.end(), gameObject);
        if (it != objectVector.end())
        {
            objectVector.erase(it);
            return true;
        }
        return false;
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