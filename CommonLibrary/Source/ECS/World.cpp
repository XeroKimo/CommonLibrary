#include "ECS/World.h"
#include "ECS/Transform.h"
#include "VectorHelpers.h"
#include <algorithm>
#include <string>

namespace CommonsLibrary
{
    World::World(std::vector<std::unique_ptr<Scene>> scenes) :
        m_sceneManager(std::move(scenes), this)
    {
        ComponentRegistry::Register<Transform>();
        AddSystem(&m_sceneManager);
    }
    void World::Update(float deltaTime)
    {
        m_sceneManager.StartGameObjects();
        m_sceneManager.UpdateGameObjects(deltaTime);
        m_sceneManager.DestroyGameObjects();
    }
  /*  ReferencePointer<GameObject> World::CreateGameObject()
    {
        ReferencePointer<GameObject> instantiatedObject = MakeReference<GameObject>(GetReferencePointer(), m_activeScene);
        instantiatedObject->InitTransform();

        ReferencePointer<GameObject> returnObject = instantiatedObject;
        m_activeScene->AddGameObject(std::move(instantiatedObject));
        return returnObject;
    }
    ReferencePointer<GameObject> World::CreateGameObject(const ReferencePointer<Transform>& parent)
    {
        ReferencePointer<GameObject> instantiatedObject = CreateGameObject();
        instantiatedObject->GetTransform()->SetParent(parent);
        return instantiatedObject;
    }
    ReferencePointer<GameObject> World::FindObject(const std::string& name) const
    {
        for (const Scene* const scene : m_loadedScenes)
        {
            ReferencePointer<GameObject> gameObject = scene->FindObject(name);
            if (gameObject)
                return gameObject;
        }
        return nullptr;
    }
    void World::LoadScene(int index)
    {
        m_activeScene = m_scenes[index].get();
        m_activeScene->LoadScene(GetReferencePointer());
        m_loadedScenes.push_back(m_activeScene);
    }
    void World::AddComponentToStart(Component* component)
    {
        m_componentsToStart.push_back(component);
    }
    void World::RemoveComponentToStart(Component* component)
    {
        RemoveFromVector(m_componentsToStart, component);
    }
    void World::SetObjectActive(const ReferencePointer<GameObject>& gameObject)
    {
        auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [=](const std::unique_ptr<Scene>& scene) { return gameObject->m_owningScene == scene.get(); });
        if (it != m_scenes.end())
        {
            it->get()->SetObjectActive(gameObject);
        }
    }*/
}