#pragma once
#include "Scene.h"
#include "ECSSystem.h"
#include <memory>

namespace CommonsLibrary
{
    class World;
    class SceneManager : public ECSSystem
    {
        friend class World;
        friend class GameObject;
    private:
        std::vector<std::unique_ptr<Scene>> m_buildScenes;
        std::unordered_map<std::string, size_t> m_buildSceneIndices;
        std::vector<std::unique_ptr<Scene>> m_loadedScenes;
        Scene* m_activeScene = nullptr;
        World* m_world;
    public:
        SceneManager(std::vector<std::unique_ptr<Scene>> scenes, World* world);

    public:
        Scene* CreateScene(std::string name);

        void LoadScene(std::string name);
        void LoadScene(size_t index);

        void UnloadScene(std::string name);
        void UnloadScene(size_t index);

        Scene* GetLoadedScene(std::string name);
        Scene* GetLoadedScene(size_t index);

        void SetActiveScene(std::string name);
        void SetActiveScene(size_t index);

        Scene* GetActiveScene() const { return m_activeScene; }

    public:
        bool TransferGameObject(const ReferencePointer<GameObject>& gameObject);

    private:
        void StartGameObjects();
        void UpdateGameObjects(float deltaTime);
        void DestroyGameObjects();

        int FindDuplicateSceneName(std::string name);

    private:
        ReferencePointer<GameObject> CreateGameObject();
    };
}