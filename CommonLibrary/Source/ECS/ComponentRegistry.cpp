#include "ECS/ComponentRegistry.h"
#include "ECS/World.h"
#include "ECS/GameObject.h"

namespace CommonsLibrary
{
    std::unordered_map<std::string, Function<ReferencePointer<Component>(const ReferencePointer<GameObject>&, const ReferencePointer<World>&)>> ComponentRegistry::m_registry;
    ReferencePointer<Component> ComponentRegistry::Create(const std::type_index& type, const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world)
    {
        return Create(type.name(), gameObject, world);
    }
    ReferencePointer<Component> ComponentRegistry::Create(const std::string& type, const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world)
    {
#if _DEBUG
        return (KeyExists(m_registry, type)) ? m_registry[type](gameObject, world) : nullptr;
#else
        return m_registry[type](gameObject, world);
#endif
    }
}