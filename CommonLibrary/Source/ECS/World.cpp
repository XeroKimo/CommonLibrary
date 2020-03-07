#include "ECS/World.h"
#include "ECS/Transform.h"
#include "VectorHelpers.h"
#include <algorithm>
#include <string>

namespace CommonsLibrary
{
    World::World(std::vector<std::unique_ptr<Scene>> scenes)
    {
        ComponentRegistry::Register<Transform>();
        m_scenes = std::move(scenes);
        
        std::string key;
        for (int i = 0; i < m_scenes.size(); i++)
        {
            key = m_scenes[i]->GetSceneName();
            if (KeyExists(m_sceneIndices, key))
            {
                m_sceneIndices[key + std::to_string(i)] = i;
            }
            else
                m_sceneIndices[key] = i;
        }
    }
    void World::Update(float deltaTime)
    {
        if (!m_componentsToStart.empty())
        {
            for (Component* component : m_componentsToStart)
            {
                component->Start();
                component->m_hasStarted = true;
            }
            m_componentsToStart.clear();
        }
        for (const auto& scene : m_loadedScenes)
        {
            scene->Update(deltaTime);
        }
    }
    ReferencePointer<GameObject> World::CreateGameObject()
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
    }
}