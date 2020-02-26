#include "ECS/GameObject.h"
#include "ECS/Transform.h"
#include "ECS/World.h"
#include "DebugTools/Logger.h"

namespace CommonsLibrary
{
    GameObject::GameObject(const ReferencePointer<World>& world)
    {
        m_world = world;

        //AddComponent<Transform>();
    }

    void CommonsLibrary::GameObject::Start()
    {
        for (const std::pair<std::type_index, std::vector<ReferencePointer<Component>>>& components : m_activeComponents)
        {
            for (const ReferencePointer<Component>& component : components.second)
            {
#ifdef _DEBUG
                try
                {
                    component->Start();
                }
                catch (std::exception e)
                {
                    Logger::Instance()->Log(e.what());
                }
#else
                component->Start();
#endif
            }
        }
    }

    void CommonsLibrary::GameObject::Update(float deltaTime)
    {
        for (const std::pair<std::type_index, std::vector<ReferencePointer<Component>>>& components : m_activeComponents)
        {
            for (const ReferencePointer<Component>& component : components.second)
            {
#ifdef _DEBUG
                try
                {
                    component->Update(deltaTime);
                }
                catch (std::exception e)
                {
                    Logger::Instance()->Log(e.what());
                }
#else
                component->Update(deltaTime);
#endif
            }
        }
    }

    void CommonsLibrary::GameObject::RemoveComponent(ReferencePointer<Component> component)
    {
        if (!component)
            return;
        if (component->GetGameObject() != GetReferencePointer())
            return;

        std::type_index key(typeid(*component));

        std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>>* mapToSearchIn = (component->IsActive()) ? &m_activeComponents : &m_inactiveComponents;
        if (KeyExists(*mapToSearchIn, key))
        {
            std::vector<ReferencePointer<Component>>* searchComponents = &((*mapToSearchIn)[key]);
            auto it = std::find(searchComponents->begin(), searchComponents->end(), component);
            if (it != searchComponents->end())
            {
                searchComponents->erase(it);
                return;
            }
        }
        else
        {
            for (auto componentPair : *mapToSearchIn)
            {
                std::vector<ReferencePointer<Component>>* searchComponents = &componentPair.second;
                auto it = std::find(searchComponents->begin(), searchComponents->end(), component);
                if (it != searchComponents->end())
                {
                    searchComponents->erase(it);
                    return;
                }
            }
        }
    }

    void CommonsLibrary::GameObject::SetComponentActive(const ReferencePointer<Component>& component, bool active)
    {
        std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>>* mapToSearchIn = (active) ? &m_inactiveComponents : &m_activeComponents;
        std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>>* mapToMoveComponent = (active) ? &m_activeComponents : &m_inactiveComponents;

        std::type_index key(typeid(*component));
        if (KeyExists(*mapToSearchIn, key))
        {
            std::vector<ReferencePointer<Component>>* searchComponents = &((*mapToSearchIn)[key]);
            std::vector<ReferencePointer<Component>>* moveComponents = &((*mapToMoveComponent)[key]);

            auto it = std::find(searchComponents->begin(), searchComponents->end(), component);
            if (it != searchComponents->end())
            {
                moveComponents->push_back(std::move(*it));
                searchComponents->erase(it);
            }
        }
        else
        {
            std::vector<ReferencePointer<Component>>* moveComponents = &((*mapToMoveComponent)[key]);
            for (auto componentPair : *mapToSearchIn)
            {
                std::vector<ReferencePointer<Component>>* searchComponents = &componentPair.second;
                auto it = std::find(searchComponents->begin(), searchComponents->end(), component);
                if (it != searchComponents->end())
                {
                    moveComponents->push_back(std::move(*it));
                    searchComponents->erase(it);
                    return;
                }
            }
        }
    }
}