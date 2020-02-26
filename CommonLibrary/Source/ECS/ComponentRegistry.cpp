#include "ECS/ComponentRegistry.h"
#include "ECS/World.h"
#include "ECS/GameObject.h"

namespace CommonsLibrary
{
    std::unordered_map<std::string, Function<ReferencePointer<Component>()>> ComponentRegistry::m_registry;
    ReferencePointer<Component> ComponentRegistry::Create(const std::type_index& type)
    {
        return Create(type.name());
    }
    ReferencePointer<Component> ComponentRegistry::Create(const std::string& type)
    {
        return (KeyExists(m_registry, type)) ? m_registry[type]() : nullptr;
    }
}