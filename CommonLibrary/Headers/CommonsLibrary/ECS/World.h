#pragma once
#include "SceneManager.h"
#include "ECSSystemManager.h"

namespace CommonsLibrary
{
    __interface ExternalSystem : public ECSSystem {};

    class World
    {
    private:
        ECSSystemManager m_systemManager;
        std::unique_ptr<SceneManager> m_sceneManager;

    public:
        World();

    public:
        void AddBuildScene(std::unique_ptr<Scene>& scene);
        void AddBuildScene(std::vector<std::unique_ptr<Scene>>& scene);

        template <class Type, std::enable_if_t<std::is_base_of_v<ECSSystem, Type>, int> = 0>
        void AddSystem(Type* system) 
        {
            m_systemManager.AddSystem<Type>(system);
        }

        ECSSystemManager* GetSystemManager() { return &m_systemManager; }

    public:
        void Update(float deltaTime);
    };
}