#pragma once
#include "Scene.h"
#include "ECSSystem.h"
#include <memory>

namespace CommonsLibrary
{
    class SceneManager
    {
        friend class GameObject;
        static SceneManager* s_sceneManager;
    private:
        std::vector<std::unique_ptr<Scene>> m_buildScenes;
        std::vector<std::unique_ptr<Scene>> m_loadedScenes;
        Scene* m_activeScene;

    private:
        SceneManager(std::vector<std::unique_ptr<Scene>> buildScenes) : 
            m_buildScenes(std::move(buildScenes)) 
        {
            LoadScene(0);
        }


    public:
        void Update(float deltaTime);

    private:
        ReferenceView<GameObject> CreateGameObject(std::string name);

    public:
        static SceneManager* CreateSceneManager(std::vector<std::unique_ptr<Scene>> buildScenes);
        static void ShutdownSceneManager();

    public:
        static Scene* LoadScene(size_t buildIndex);
    };
}