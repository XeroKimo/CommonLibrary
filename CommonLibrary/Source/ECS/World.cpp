#include "ECS/World.h"
#include "ECS/Transform.h"
#include "VectorHelpers.h"

namespace CommonsLibrary
{
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
        for (const auto& scene : m_scenes)
        {
            scene->Update(deltaTime);
        }
    }
    ReferencePointer<GameObject> World::Instantiate()
    {
        ReferencePointer<GameObject> instantitatedObject = MakeReference<GameObject>(GetReferencePointer(), m_activeScene);
        ReferencePointer<GameObject> returnObject = instantitatedObject;
        m_activeScene->AddGameObject(std::move(instantitatedObject));
        return returnObject;
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