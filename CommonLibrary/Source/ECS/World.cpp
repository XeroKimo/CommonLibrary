#include "CommonsLibrary/ECS/World.h"
#include "CommonsLibrary/ECS.h"

namespace CommonsLibrary
{
    World::World()
    {
        ComponentRegistry::Register<Transform>();
        m_sceneManager = std::make_unique<SceneManager>(this);
        AddSystem(m_sceneManager.get());
    }

    void World::AddBuildScene(std::unique_ptr<Scene>& scene)
    {
        m_sceneManager->AddBuildScene(scene);
    }
    void World::AddBuildScene(std::vector<std::unique_ptr<Scene>>& scene)
    {
        m_sceneManager->AddBuildScene(scene);
    }
    void World::Update(float deltaTime)
    {
        m_sceneManager->StartGameObjects();
        m_sceneManager->UpdateGameObjects(deltaTime);
        m_sceneManager->DestroyGameObjects();
    }
}