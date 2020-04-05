#include "CommonsLibrary/ECS/SceneManager.h"
#include "CommonsLibrary/DebugTools/Assert.h"
#include <string>

namespace CommonsLibrary
{
    CommonsLibrary::SceneManager::SceneManager(std::vector<std::unique_ptr<Scene>> scenes, World* world) :
        m_buildScenes(std::move(scenes)),
        m_world(world)
    {
        assertError(!m_buildScenes.empty(), "There are 0 scenes in the build");

        std::string key;
        for (size_t i = 0; i < m_buildScenes.size(); i++)
        {
            key = m_buildScenes[i]->GetSceneName();
            assertError(!KeyExists(m_buildSceneIndices, key), "Scene name already exists");
            m_buildSceneIndices[key] = i;
        }
    }

    Scene* CommonsLibrary::SceneManager::CreateScene(std::string name)
    {
        m_loadedScenes.push_back(std::make_unique<Scene>(name));
        m_loadedScenes.back()->LoadScene(m_world);
        return m_loadedScenes.back().get();
    }

    void SceneManager::LoadScene(std::string name)
    {
        if (KeyExists(m_buildSceneIndices, name))
            LoadScene(m_buildSceneIndices[name]);
    }

    void SceneManager::LoadScene(size_t index)
    {
        if (index > m_buildScenes.size())
            return;

        m_buildScenes[index]->LoadScene(m_world);
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(*m_buildScenes[index]);
        m_buildScenes[index]->UnloadScene();
        int duplicateCount = FindDuplicateSceneName(scene->GetSceneName());
        if (duplicateCount > 0)
            scene->m_sceneName += std::to_string(duplicateCount);
        m_activeScene = scene.get();

        m_loadedScenes.push_back(std::move(scene));
    }

    void SceneManager::UnloadScene(std::string name)
    {
        for (size_t i = 0; i < m_loadedScenes.size(); i++)
        {
            if (m_loadedScenes[i]->GetSceneName() == name)
            {
                UnloadScene(i);
                return;
            }
        }
    }

    void SceneManager::UnloadScene(size_t index)
    {
        if (m_loadedScenes.size() == 1)
            return;
        if (m_loadedScenes.size() < index)
            return;

        if (m_activeScene == m_loadedScenes[index].get())
            m_activeScene = nullptr;

        m_loadedScenes.erase(m_loadedScenes.begin() + index);
        m_activeScene = m_loadedScenes[0].get();
    }

    Scene* SceneManager::GetLoadedScene(std::string name)
    {
        for (const auto& scene : m_loadedScenes)
        {
            if (scene->GetSceneName() == name)
                return scene.get();
        }
        return nullptr;
    }

    Scene* SceneManager::GetLoadedScene(size_t index)
    {
        if (index > m_loadedScenes.size())
            return nullptr;
        return m_loadedScenes[index].get();
    }

    void SceneManager::SetActiveScene(std::string name)
    {
        for (size_t i = 0; i < m_loadedScenes.size(); i++)
        {
            if (m_loadedScenes[i]->GetSceneName() == name)
            {
                SetActiveScene(i);
                return;
            }
        }
    }

    void SceneManager::SetActiveScene(size_t index)
    {
        if (index > m_loadedScenes.size())
            return;
        Scene* sceneToBeActive = m_loadedScenes[index].get();
        if (sceneToBeActive != m_activeScene)
            m_activeScene = sceneToBeActive;
    }

    bool SceneManager::TransferGameObject(const ReferencePointer<GameObject>& gameObject)
    {
        if (gameObject->m_scene == m_activeScene)
            return false;

        m_activeScene->PlaceGameObject(std::move(gameObject->m_scene->ExtractGameObject(gameObject)));

        return true;
    }

    void SceneManager::StartGameObjects()
    {
        for (const auto& scene : m_loadedScenes)
            scene->StartGameObjects();
    }

    void SceneManager::UpdateGameObjects(float deltaTime)
    {
        for (const auto& scene : m_loadedScenes)
            scene->UpdateGameObjects(deltaTime);
    }
    void SceneManager::DestroyGameObjects()
    {
        for (const auto& scene : m_loadedScenes)
            scene->DestroyGameObjects();
    }

    int SceneManager::FindDuplicateSceneName(std::string name)
    {
        int duplicateCount = 0;
        for (const auto& loadedScene : m_loadedScenes)
        {
            std::string sceneName = loadedScene->GetSceneName();
            if (sceneName.size() < name.size())
                continue;
            if (loadedScene->GetSceneName().substr(0, name.size()) == name)
                duplicateCount++;
        }
        return duplicateCount;
    }
    ReferencePointer<GameObject> SceneManager::CreateGameObject()
    {
        return m_activeScene->CreateGameObject();
    }
}