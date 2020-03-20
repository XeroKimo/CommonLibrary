#include "ECS/ComponentRegistry.h"
#include "ECS/World.h"
#include "ECS/GameObject.h"
#include "DebugTools/Assert.h"
#include <string>
#include <cstdlib>

namespace CommonsLibrary
{
    namespace ComponentRegistry
    {
        namespace Internal
        {
            std::unordered_map<std::string, Function<ReferencePointer<Component>(const ReferencePointer<GameObject>&)>> g_registry;
        }
        ReferencePointer<Component> ComponentRegistry::Create(const std::type_index& type, const ReferencePointer<GameObject>& gameObject)
        {
            return Create(type.name(), gameObject);
        }
        ReferencePointer<Component> ComponentRegistry::Create(const std::string& type, const ReferencePointer<GameObject>& gameObject)
        {
#if _DEBUG
            if (KeyExists(Internal::g_registry, type))
            {
                return Internal::g_registry[type](gameObject);
            }
            else
            {
                wchar_t* wideString = nullptr;
                mbtowc(wideString, type.c_str(), type.size());

                std::wstring errorMessage = wideString;
                errorMessage += L" is no reigstered";
                _wassert(errorMessage.c_str(), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
                return nullptr;
            }
#else
            return Internal::g_registry[type](gameObject);
#endif
        }
    }
}
