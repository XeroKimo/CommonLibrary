#include "CommonsLibrary/ECS/Component.h"


namespace CommonsLibrary
{
    ReferencePointer<Component> ComponentRegistry::CreateComponent(std::type_index type, const ReferencePointer<GameObject>& gameObject)
    {
        std::string name = type.name();
        return CreateComponent(name.substr(name.find(' ') + 1), gameObject);
    }

    ReferencePointer<Component> ComponentRegistry::CreateComponent(std::string name, const ReferencePointer<GameObject>& gameObject)
    {
        return GetRegisteredComponents()[name](gameObject);
    }

    std::unordered_map<std::string, ComponentRegistry::CreateCallback>& ComponentRegistry::GetRegisteredComponents()
    {
        static std::unordered_map<std::string, CreateCallback> s_registeredComponents;
        return s_registeredComponents;
    }
}
