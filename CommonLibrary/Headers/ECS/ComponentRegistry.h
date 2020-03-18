#pragma once
#include "Component.h"
#include "../UnorderedMapHelpers.h"
#include "Delegates/FunctionPointer.h"
#include "ReferencePointer.h"
#include <typeindex>

namespace CommonsLibrary
{
    class GameObject;
    class World;

    namespace ComponentRegistry
    {
        namespace Internal
        {
            extern std::unordered_map<std::string, Function<ReferencePointer<Component>(const ReferencePointer<GameObject>&, const ReferencePointer<World>&)>> g_registry;
            template <class T>
            inline ReferencePointer<Component> CreateComponent(const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world)
            {
                return MakeReference<T>(gameObject, world);
            }
        }
        template <class T, class = std::enable_if_t<std::is_base_of_v<Component, T>>>
        inline void Register()
        {
            std::string name = typeid(T).name();
            if (!KeyExists(Internal::g_registry, name))
                Internal::g_registry[name] = Internal::CreateComponent<T>;
        }


        ReferencePointer<Component> Create(const std::type_index& type, const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world);
        ReferencePointer<Component> Create(const std::string& type, const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world);
    }
}