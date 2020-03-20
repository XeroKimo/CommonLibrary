#include "CommonsLibrary/ECS/World.h"
#include "CommonsLibrary/ECS/ComponentRegistry.h"
#include "CommonsLibrary/ECS/Transform.h"

namespace CommonsLibrary
{
    World::World(std::vector<std::unique_ptr<Scene>> scenes)
    {
        ComponentRegistry::Register<Transform>();
        m_sceneManager = std::make_unique<SceneManager>(std::move(scenes), this);
        AddSystem(m_sceneManager.get());
    }
    void World::Update(float deltaTime)
    {
        m_sceneManager->StartGameObjects();
        m_sceneManager->UpdateGameObjects(deltaTime);
        m_sceneManager->DestroyGameObjects();
    }
}