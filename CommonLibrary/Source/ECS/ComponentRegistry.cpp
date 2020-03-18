#include "ECS/ComponentRegistry.h"
#include "ECS/World.h"
#include "ECS/GameObject.h"
#include <assert.h>
#include <string>

namespace CommonsLibrary
{
    namespace ComponentRegistry
    {
        namespace Internal
        {
            std::unordered_map<std::string, Function<ReferencePointer<Component>(const ReferencePointer<GameObject>&, const ReferencePointer<World>&)>> g_registry;
        }
        ReferencePointer<Component> ComponentRegistry::Create(const std::type_index& type, const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world)
        {
            return Create(type.name(), gameObject, world);
        }
        ReferencePointer<Component> ComponentRegistry::Create(const std::string& type, const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world)
        {
#if _DEBUG
            if (KeyExists(Internal::g_registry, type))
            {
                return Internal::g_registry[type](gameObject, world);
            }
            else
            {
                _wassert(L"Component not registered", _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
                return nullptr;
            }
#else
            return m_registry[type](gameObject, world);
#endif
        }
    }
}
