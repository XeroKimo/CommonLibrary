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
        using CreateCallback = Delegate<ReferencePointer<Component>(const ReferencePointer<GameObject>&)>;
    public:
        template <class Type>
        static void RegisterComponent()
        {
            std::string name = typeid(Type).name();
            name = name.substr(name.find(' ') + 1);
            GetRegisteredComponents()[name].Bind<CreateComponentCallback<Type>>();
        }

        static ReferencePointer<Component> CreateComponent(std::type_index type, const ReferencePointer<GameObject>& gameObject);
        static ReferencePointer<Component> CreateComponent(std::string name, const ReferencePointer<GameObject>& gameObject);

        static void RegisteredStaticComponent(Component* component, void** staticPointer);
        static void ClearRegisteredStatics();
        static bool IsComponentRegistered(std::type_index type);
    private:
        template <class Type>
        static ReferencePointer<Component> CreateComponentCallback(const ReferencePointer<GameObject>& gameObject)
        {
            return MakeReference<Type>(gameObject);
        }

        static std::unordered_map<std::string, CreateCallback>& GetRegisteredComponents();
        static std::vector<std::pair<Component*, void**>>& GetStaticComponents();
    };
}