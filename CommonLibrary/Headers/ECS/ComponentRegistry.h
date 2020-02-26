#pragma once
#include "Component.h"
#include "../UnorderedMapHelpers.h"
#include "Delegates/FunctionPointer.h"
#include "ReferencePointer.h"
#include <typeindex>

namespace CommonsLibrary
{
    class ComponentRegistry
    {
        class GameObject;
        class World;
    private:
        static std::unordered_map<std::string, Function<ReferencePointer<Component>()>> m_registry;

    public:
        template <class T, class = std::enable_if_t<std::is_base_of_v<Component, T>>>
        static void Register()
        {
            std::string name = typeid(T).name();
            if (!KeyExists(m_registry, name))
                m_registry[name] = &ComponentRegistry::CreateComponent<T>;
        }

        static ReferencePointer<Component> Create(const std::type_index& type);
        static ReferencePointer<Component> Create(const std::string& type);

    private:
        template <class T, class = std::enable_if_t<std::is_base_of_v<Component, T>>>
        static ReferencePointer<T> CreateComponent()
        {
            return MakeReference<T>(nullptr, nullptr);
        }
    };
}