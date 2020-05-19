#include "CommonsLibrary/ECS/SceneManager.h"
#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/DebugTools/Assert.h"
#include <string>

namespace CommonsLibrary
{
    SceneManager* SceneManager::s_sceneManager = nullptr;

    Scene* SceneManager::LoadScene(size_t buildIndex)
    {
        using namespace std;

        s_sceneManager->m_loadedScenes.clear();

        unique_ptr<Scene> copy(new Scene("temp"));
        s_sceneManager->m_activeScene = copy.get();
        s_sceneManager->m_buildScenes[buildIndex]->Load();

        s_sceneManager->m_activeScene->MergeScene(s_sceneManager->m_buildScenes[buildIndex].get());
        s_sceneManager->m_activeScene->m_isLoaded = true;
        s_sceneManager->m_activeScene->Awake();

        s_sceneManager->m_loadedScenes.push_back(std::move(copy));

        return s_sceneManager->m_loadedScenes.back().get();
    }
    void SceneManager::Update(float deltaTime)
    {
        for(auto& scene : m_loadedScenes)
        {
            scene->Start();
        }
        for(auto& scene : m_loadedScenes)
        {
            scene->Update(deltaTime);
        }
    }
    ReferencePointer<GameObject> SceneManager::CreateGameObject(std::string name)
    {
        return m_activeScene->CreateGameObject(name);
    }
    SceneManager* SceneManager::CreateSceneManager(std::vector<std::unique_ptr<Scene>> buildScenes)
    {
        if(s_sceneManager)
            return nullptr;
        s_sceneManager = new SceneManager(std::move(buildScenes));
        return s_sceneManager;
    }
    void SceneManager::ShutdownSceneManager()
    {
        delete s_sceneManager;
        s_sceneManager = nullptr;
    }
}