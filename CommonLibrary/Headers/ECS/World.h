#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "GameObject.h"
#include "ECS/Scene.h"
#include <memory>

namespace CommonsLibrary
{
    class World : ReferenceFromThis<World>
    {
        friend class GameObject;
    private:
        std::vector<std::unique_ptr<Scene>> m_scenes;
        Scene* m_activeScene = nullptr;

        std::vector<Component*> m_componentsToStart;
    public:
        void Update(float deltaTime);
    public:
        ReferencePointer<GameObject> Instantiate();

    private:
        void AddComponentToStart(Component* component);
        void RemoveComponentToStart(Component* component);

        void SetObjectActive(const ReferencePointer<GameObject>& gameObject);
    };
}