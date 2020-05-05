#pragma once
#include "Scene.h"
#include "ECSSystem.h"
#include <memory>

namespace CommonsLibrary
{
    class SceneManager
    {
        friend class GameObject;
    private:
        std::vector<std::unique_ptr<Scene>> m_buildScenes;
        std::vector<std::unique_ptr<Scene>> m_loadedScenes;
        Scene* m_activeScene;
    public:
        Scene* LoadScene(size_t buildIndex);

    private:
        void Start();
        void Update(float deltaTime);

    private:
        void AddBuildScene(std::unique_ptr<Scene> scene);

    private:
        ReferencePointer<GameObject> CreateGameObject(std::string name);
    };
}