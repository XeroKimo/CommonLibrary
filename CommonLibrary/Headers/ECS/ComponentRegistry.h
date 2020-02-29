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
    class ComponentRegistry
    {
    private:
        static std::unordered_map<std::string, Function<ReferencePointer<Component>(const ReferencePointer<GameObject>&, const ReferencePointer<World>&)>> m_registry;

    public:
        template <class T, class = std::enable_if_t<std::is_base_of_v<Component, T>>>
        static void Register()
        {
            std::string name = typeid(T).name();
            if (!KeyExists(m_registry, name))
                m_registry[name] = &ComponentRegistry::CreateComponent;
        }

        static ReferencePointer<Component> Create(const std::type_index& type, const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world);
        static ReferencePointer<Component> Create(const std::string& type, const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world);

    private:
        static ReferencePointer<Component> CreateComponent(const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world)
        {
            return MakeReference<Component>(gameObject, world);
        }
    };
}