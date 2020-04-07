#include "CommonsLibrary/ECS.h"
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
        m_updateGameObjects.swap(other.m_updateGameObjects);
        m_gameObjectsToDestroy.swap(other.m_gameObjectsToDestroy);
        m_sceneName = other.m_sceneName;
        m_world = other.m_world;
    }
    Scene::~Scene()
    {
        UnloadScene();
    }
    ReferencePointer<GameObject> Scene::FindObject(const std::string& name) const
    {
        return FindObject(m_updateGameObjects, name);
    }
    ReferencePointer<GameObject> Scene::CreateGameObject()
    {
        if (!m_world)
            return nullptr;
        return m_world->GetSystemManager()->GetSystem<SceneManager>()->CreateGameObject();
        //ReferencePointer<GameObject> createdObject = MakeReference<GameObject>(this);
        //m_updateGameObjects.push_back(std::move(createdObject));
        //return m_updateGameObjects.back();
    }
    void Scene::UnloadScene()
    {
        m_updateGameObjects.clear();
        m_gameObjectsToDestroy.clear();
    }
    void Scene::StartGameObjects()
    {
        for (const auto& gameObjects : m_updateGameObjects)
        {
            gameObjects->Start();
        }
    }
    void Scene::UpdateGameObjects(float deltaTime)
    {
        for (const auto& gameObject : m_updateGameObjects)
            gameObject->Update(deltaTime);

        for (const auto& gameObject : m_updateGameObjects)
            gameObject->OnDestroy();

    }
    void Scene::DestroyGameObjects()
    {
        if (!m_gameObjectsToDestroy.empty())
        {
            for (const auto& gameObjects : m_gameObjectsToDestroy)
            {
                gameObjects->OnDestroy();
            }
            m_gameObjectsToDestroy.clear();
        }
    }
    void Scene::DestroyGameObject(const ReferencePointer<GameObject>& gameObject)
    {
        FindObjectToDelete(m_updateGameObjects, gameObject);
    }
    void Scene::PlaceGameObject(ReferencePointer<GameObject> gameObject)
    {
        m_updateGameObjects.push_back(std::move(gameObject));
    }
    ReferencePointer<GameObject> Scene::ExtractGameObject(const ReferencePointer<GameObject>& gameObject)
    {
        auto it = std::find(m_updateGameObjects.begin(), m_updateGameObjects.end(), gameObject);

        ReferencePointer<GameObject> extractedObject = std::move(*it);
        m_updateGameObjects.erase(it);

        return extractedObject;
    }
    bool Scene::FindObjectToDelete(std::vector<ReferencePointer<GameObject>>& objectVector, const ReferencePointer<GameObject>& gameObject)
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
    ReferencePointer<GameObject> Scene::FindObject(const std::vector<ReferencePointer<GameObject>>& objectVector, const std::string& name) const
    {
        for (const auto& gameObject : objectVector)
        {
            if (gameObject->name == name)
                return gameObject;
        }
        return nullptr;
    }
}