#pragma once
#include "CommonsLibrary/Miscellaneous/ReferencePointer.h"
#include "CommonsLibrary/Delegates/Delegate.h"

#include <unordered_map>
#include <typeindex>

namespace CommonsLibrary
{
    class Component;
    class GameObject;


    class ComponentRegistry
    {
        friend class ComponentManager;

        using CreateCallback = Delegate<ReferencePointer<Component>(const ReferencePointer<GameObject>&)>;

    public:
        template <class Type, std::enable_if_t<std::is_base_of_v<Component, Type>, int> = 0>
        static void RegisterComponent()
        {
            std::string name = typeid(Type).name();
            name = name.substr(name.find(' ') + 1);
            GetRegisteredComponents()[name].Bind<CreateComponentCallback<Type>>();
        }

        static bool IsComponentRegistered(std::type_index type)
        {
            std::string name = type.name();
            name = name.substr(name.find(' ') + 1);
            return GetRegisteredComponents().count(name) != 0;
        }

    public:
        static ReferencePointer<Component> CreateComponent(std::type_index type, const ReferencePointer<GameObject>& gameObject);
        static ReferencePointer<Component> CreateComponent(std::string name, const ReferencePointer<GameObject>& gameObject);

    private:
        template <class Type>
        static ReferencePointer<Component> CreateComponentCallback(const ReferencePointer<GameObject>& gameObject)
        {
            return MakeReference<Type>(gameObject);
        }

        static std::unordered_map<std::string, CreateCallback>& GetRegisteredComponents();
    };
}