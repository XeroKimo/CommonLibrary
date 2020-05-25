#include "..\..\Headers\CommonsLibrary\ECS\ECS.h"
//#include "CommonsLibrary/ECS/Component.h"


namespace CommonsLibrary
{
    //ReferencePointer<Component> ComponentRegistry::CreateComponent(std::type_index type, const ReferenceView<GameObject>& gameObject)
    //{
    //    std::string name = type.name();
    //    return CreateComponent(name.substr(name.find(' ') + 1), gameObject);
    //}

    //ReferencePointer<Component> ComponentRegistry::CreateComponent(std::string name, const ReferenceView<GameObject>& gameObject)
    //{
    //    return GetRegisteredComponents()[name](gameObject);
    //}

    namespace ComponentRegistry
    {
        std::unordered_map<std::string, std::unique_ptr<ComponentManagerBase>>& GetRegisteredComponents()
        {
            static std::unordered_map<std::string, std::unique_ptr<ComponentManagerBase>> registeredComponents;
            return registeredComponents;
        }

        std::pair<std::string, std::unique_ptr<ComponentManagerBase>> CreateComponentManager(std::type_index type)
        {
            return CreateComponentManager(type.name());
        }

        std::pair<std::string, std::unique_ptr<ComponentManagerBase>> CreateComponentManager(std::string type)
        {
            return std::pair(type, std::unique_ptr<ComponentManagerBase>(GetRegisteredComponents()[type]->CreateCopy()));
        }
    }

    //std::unordered_map<std::string, ComponentRegistry::CreateCallback>& ComponentRegistry::GetRegisteredComponents()
    //{
    //    static std::unordered_map<std::string, CreateCallback> s_registeredComponents;
    //    return s_registeredComponents;
    //}
}
