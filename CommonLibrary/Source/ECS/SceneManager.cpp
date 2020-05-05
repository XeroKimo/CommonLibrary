#include "CommonsLibrary/ECS/SceneManager.h"
#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/DebugTools/Assert.h"
#include <string>

namespace CommonsLibrary
{
    Scene* SceneManager::LoadScene(size_t buildIndex)
    {
        using namespace std;

        m_loadedScenes.clear();

        unique_ptr<Scene> copy(new Scene("temp"));
        m_activeScene = copy.get();
        m_buildScenes[buildIndex]->Load();

        m_activeScene->MergeScene(m_buildScenes[buildIndex].get());
        m_activeScene->m_isLoaded = true;
        m_activeScene->Awake();

        m_loadedScenes.push_back(std::move(copy));

        return m_loadedScenes.back().get();
    }
    void SceneManager::Start()
    {
        for(auto& scene : m_loadedScenes)
        {
            scene->Start();
        }
    }
    void SceneManager::Update(float deltaTime)
    {
        for(auto& scene : m_loadedScenes)
        {
            scene->Update(deltaTime);
        }
    }
    void SceneManager::AddBuildScene(std::unique_ptr<Scene> scene)
    {
        m_buildScenes.push_back(std::move(scene));
    }
    ReferencePointer<GameObject> SceneManager::CreateGameObject(std::string name)
    {
        return m_activeScene->CreateGameObject(name);
    }
}