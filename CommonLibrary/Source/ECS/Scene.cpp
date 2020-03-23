#include "CommonsLibrary/ECS/Scene.h"
#include "CommonsLibrary/StdHelpers/VectorHelpers.h"

namespace CommonsLibrary
{
    Scene::Scene(std::string sceneName) :
        m_world(nullptr)
    {
        m_sceneName = std::move(sceneName);
    }
    Scene::Scene(Scene& other)
    {
        m_activeGameObjects.swap(other.m_activeGameObjects);
        m_inactiveGameObjects.swap(other.m_inactiveGameObjects);
        m_gameObjectsToDestroy.swap(other.m_gameObjectsToDestroy);
        m_gameObjectsToStart.swap(other.m_gameObjectsToStart);
        m_sceneName = other.m_sceneName;
        m_world = other.m_world;
    }
    Scene::~Scene()
    {
        UnloadScene();
    }
    ReferencePointer<GameObject> Scene::FindObject(const std::string& name) const
    {
        ReferencePointer<GameObject> objectToFind = FindObject(m_activeGameObjects, name);
        return (objectToFind) ? objectToFind : FindObject(m_inactiveGameObjects, name);
    }
    ReferencePointer<GameObject> Scene::CreateGameObject()
    {
        if (!m_world)
            return nullptr;
        ReferencePointer<UpdateableGameObject> createdObject = MakeReference<UpdateableGameObject>(this);
        m_activeGameObjects.push_back(std::move(createdObject));
        return m_activeGameObjects.back();
    }
    void Scene::UnloadScene()
    {
        m_activeGameObjects.clear();
        m_inactiveGameObjects.clear();
        m_gameObjectsToDestroy.clear();
        m_gameObjectsToStart.clear();
    }
    void Scene::StartGameObjects()
    {
        if (!m_gameObjectsToStart.empty())
        {
            for (const auto& gameObjects : m_gameObjectsToStart)
            {
                gameObjects->Start();
            }
            m_gameObjectsToStart.clear();
        }
    }
    void Scene::UpdateGameObjects(float deltaTime)
    {
        for (const auto& gameObject : m_activeGameObjects)
            gameObject->Update(deltaTime);
    }
    void Scene::DestroyGameObjects()
    {
        if (!m_gameObjectsToDestroy.empty())
        {
            for (const auto& gameObjects : m_gameObjectsToStart)
            {
                gameObjects->OnDestroy();
            }
            m_gameObjectsToDestroy.clear();
        }
    }
    void Scene::DestroyGameObject(const ReferencePointer<GameObject>& gameObject)
    {
        if (!FindObjectToDelete(m_activeGameObjects, ReferencePointerStaticCast<UpdateableGameObject>(gameObject)))
            FindObjectToDelete(m_inactiveGameObjects, ReferencePointerStaticCast<UpdateableGameObject>(gameObject));
    }
    void Scene::SetObjectActive(const ReferencePointer<GameObject>& gameObject)
    {
        bool isActive = gameObject->GetActiveWorld();
        auto& toRemoveVector = (isActive) ? m_inactiveGameObjects : m_activeGameObjects;
        auto& toAddVector = (isActive) ? m_activeGameObjects : m_inactiveGameObjects;

        auto it = std::find(toRemoveVector.begin(), toRemoveVector.end(), gameObject);
        toAddVector.push_back(std::move(*it));
        toRemoveVector.erase(it);
    }
    void Scene::SetGameObjectToStart(const ReferencePointer<GameObject>& gameObject)
    {
        m_gameObjectsToStart.push_back(ReferencePointerStaticCast<UpdateableGameObject>(gameObject));
    }
    bool Scene::FindObjectToDelete(std::vector<ReferencePointer<UpdateableGameObject>>& objectVector, const ReferencePointer<UpdateableGameObject>& gameObject)
    {
        auto it = std::find(objectVector.begin(), objectVector.end(), gameObject);
        if (it != objectVector.end())
        {
            m_gameObjectsToDestroy.push_back(std::move(*it));
            objectVector.erase(it);
            return true;
        }
        return false;
    }
    ReferencePointer<GameObject> Scene::FindObject(const std::vector<ReferencePointer<UpdateableGameObject>>& objectVector, const std::string& name) const
    {
        for (const auto& gameObject : objectVector)
        {
            if (gameObject->name == name)
                return gameObject;
        }
        return nullptr;
    }
}