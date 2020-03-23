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

    public:
        void Start();
        void Update(float deltaTime);
        void OnGameObjectDestroyed();

    public:
        void AddComponent(Component* component);
        void RemoveComponent(ReferencePointer<Component> component);
        void SetComponentActive(Component* component);

    private:
        void CallOnDestroy(const std::vector<Component*>& components);
    };
}