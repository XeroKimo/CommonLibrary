#pragma once
#include "Component.h"
#include <vector>

namespace CommonsLibrary
{
    struct UpdateableComponents
    {
        std::vector<Component*> activeComponents;
        std::vector<Component*> inactiveComponents;
        std::vector<Component*> componentsToStart;
        std::vector<ReferencePointer<Component>> componentsToDestroy;

    public:
        void Start();
        void Update(float deltaTime);
        void CleanUp();
        void OnGameObjectDestroyed();

    public:
        void AddComponent(Component* component);
        void RemoveComponent(ReferencePointer<Component> component);
        void SetComponentActive(Component* component);

    private:
        void CallOnDestroy(const std::vector<Component*>& components);
    };
}