#pragma once
#include "Component.h"
#include "CommonsLibrary/StdHelpers/UnorderedMapHelpers.h"
#include "CommonsLibrary/Delegates/FunctionPointer.h"
#include "CommonsLibrary/Miscellaneous/ReferencePointer.h"
#include <typeindex>

namespace CommonsLibrary
{
    class GameObject;
    class Scene;

    class ComponentRegistry
    {
        ComponentRegistry() = delete;

        inline static std::unordered_map<std::string, Function<ReferencePointer<Component>(const ReferencePointer<GameObject>&)>> m_registry;

    private:           
        template <class T>
        static ReferencePointer<Component> CreateComponent(const ReferencePointer<GameObject>& gameObject)
        {
            return MakeReference<T>(gameObject);
        }

    public:
        template <class T, class = std::enable_if_t<std::is_base_of_v<Component, T>>>
        static void Register()
        {
            std::string name = typeid(T).name();
            if (!KeyExists(m_registry, name))
                m_registry[name] = CreateComponent<T>;
        }

        static ReferencePointer<Component> Create(const std::type_index& type, const ReferencePointer<GameObject>& gameObject);
        static ReferencePointer<Component> Create(const std::string& type, const ReferencePointer<GameObject>& gameObject);
    };
}