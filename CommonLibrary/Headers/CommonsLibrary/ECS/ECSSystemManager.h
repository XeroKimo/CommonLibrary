#pragma once
#include "ECSSystem.h"
#include "CommonsLibrary/StdHelpers/UnorderedMapHelpers.h"

namespace CommonsLibrary
{
    class ECSSystemManager
    {
        friend class World;
        std::unordered_map<std::string, ECSSystem*> m_systems;

    private:
        template <class Type, std::enable_if_t<std::is_base_of_v<ECSSystem, Type>, int> = 0>
        void AddSystem(Type* system)
        {
            m_systems[typeid(Type).name()] = system;
        }

    public:
        template <class Type, std::enable_if_t<std::is_base_of_v<ECSSystem, Type>, int> = 0>
        Type* GetSystem()
        {
            std::string key = typeid(Type).name();
            if (KeyExists(m_systems, key))
                return static_cast<Type*>(m_systems[key]);
            else
                return nullptr;
        }
    };
}