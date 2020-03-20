#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "ECSSystem.h"

namespace CommonsLibrary
{
    __interface ExternalSystem : public ECSSystem {};

    class World
    {
        friend class GameObject;
    private:
        std::unordered_map<std::type_index, ECSSystem*> m_systems;
        std::unique_ptr<SceneManager> m_sceneManager;

    public:
        World(std::vector<std::unique_ptr<Scene>> scenes);

    public:
        template <class Type, class Enable = std::enable_if_t<std::is_base_of_v<ECSSystem, Type>>>
        void AddSystem(Type* system) 
        {
            std::type_index key(typeid(Type));
            if (!KeyExists(m_systems, key))
                m_systems[key] = system;
        }

        template <class Type, class Enable = std::enable_if_t<std::is_base_of_v<ECSSystem, Type>>>
        Type* GetSystem() 
        {
            std::type_index key(typeid(Type));
            if (KeyExists(m_systems, key))
                return static_cast<Type*>(m_systems[key]);
            else
                return nullptr;
        }
    public:
        void Update(float deltaTime);
    };
}