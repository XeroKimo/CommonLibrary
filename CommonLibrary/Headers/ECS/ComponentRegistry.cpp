#include "ComponentRegistry.h"

namespace CommonsLibrary
{
    std::unordered_map<std::string, FunctionPointer<ReferencePointer<Component>>> ComponentRegistry::m_registry;
    ReferencePointer<Component> ComponentRegistry::Create(const std::type_index& type)
    {
        return Create(type.name());
    }
    ReferencePointer<Component> ComponentRegistry::Create(const std::string& type)
    {
        return (KeyExists(m_registry, type)) ? m_registry[type]() : nullptr;
    }
}