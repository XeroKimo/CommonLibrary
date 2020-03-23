#include "CommonsLibrary/ECS/ComponentMap.h"

namespace CommonsLibrary
{
    ReferencePointer<Component> ComponentMap::RemoveComponent(const ReferencePointer<Component>& component)
    {
        std::type_index key(typeid(*component));
        auto& componentVector = m_componentMap[key];
        auto it = std::find(componentVector.begin(), componentVector.end(), component);
        if (it != componentVector.end())
        {
            ReferencePointer<Component> component = std::move(*it);
            componentVector.erase(it);
            return component;
        }
        return ReferencePointer<Component>();
    }
}