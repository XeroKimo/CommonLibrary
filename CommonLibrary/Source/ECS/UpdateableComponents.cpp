#include "CommonsLibrary/ECS/UpdateableComponents.h"

namespace CommonsLibrary
{
    void UpdateableComponents::Start()
    {
        if (!componentsToStart.empty())
        {
            for (const auto component : componentsToStart)
                component->Start();
            componentsToStart.clear();
        }
    }
    void UpdateableComponents::Update(float deltaTime)
    {
        for (const auto component : activeComponents)
        {
            component->Update(deltaTime);
        }
    }

    void UpdateableComponents::OnGameObjectDestroyed()
    {
        CallOnDestroy(activeComponents);
        CallOnDestroy(inactiveComponents);
    }
    void UpdateableComponents::AddComponent(Component* component)
    {
        if (component->IsActive())
            activeComponents.push_back(component);
        else
            inactiveComponents.push_back(component);
    }
    void UpdateableComponents::RemoveComponent(ReferencePointer<Component> component)
    {
        if (!component)
            return;

        if (component->IsActive())
        {
            activeComponents.erase(std::find(activeComponents.begin(), activeComponents.end(), component.Get()));
        }
        else
        {
            inactiveComponents.erase(std::find(inactiveComponents.begin(), inactiveComponents.end(), component.Get()));
        }
    }
    void UpdateableComponents::SetComponentActive(Component* component)
    {
        std::vector<Component*>& currentVector = (component->IsActive()) ? activeComponents : inactiveComponents;
        std::vector<Component*>& vectorToMove = (component->IsActive()) ? inactiveComponents : activeComponents;

        auto it = std::find(currentVector.begin(), currentVector.end(), component);
        vectorToMove.push_back(*it);
        currentVector.erase(it);

        bool wasInactive = !component->IsActive();
        if (wasInactive)
        {
            if (!component->m_hasStarted)
                componentsToStart.push_back(component);
        }
    }
    void UpdateableComponents::CallOnDestroy(const std::vector<Component*>& components)
    {
        for (const auto component : components)
        {
            component->OnDestroy();
        }
    }
}