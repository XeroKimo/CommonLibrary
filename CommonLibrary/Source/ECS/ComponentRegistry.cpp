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

    void ComponentRegistry::RegisteredStaticComponent(Component* component, void** staticPointer)
    {
        GetStaticComponents().push_back({ component, staticPointer });
    }

    void ComponentRegistry::ClearRegisteredStatics()
    {
        auto& s_components = GetStaticComponents();
        for(auto component : s_components)
        {
            delete component.first;
            (*component.second) = nullptr;
        }
        s_components.clear();
    }

    bool ComponentRegistry::IsComponentRegistered(std::type_index type)
    {
        std::string name = type.name();
        name = name.substr(name.find(' ') + 1);
        return GetRegisteredComponents().count(name) > 0;
    }

    std::unordered_map<std::string, ComponentRegistry::CreateCallback>& ComponentRegistry::GetRegisteredComponents()
    {
        static std::unordered_map<std::string, CreateCallback> s_registeredComponents;
        return s_registeredComponents;
    }

    std::vector<std::pair<Component*, void**>>& ComponentRegistry::GetStaticComponents()
    {
        static std::vector<std::pair<Component*, void**>> s_components;
        return s_components;
    }
}
