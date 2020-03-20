#include "CommonsLibrary/ECS/ComponentRegistry.h"
#include "CommonsLibrary/ECS/World.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/DebugTools/Assert.h"

#include <locale>
#include <codecvt>
#include <string>

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
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

                std::wstring errorMessage = converter.from_bytes(type);
                errorMessage += L" is not reigstered";
                _wassert(errorMessage.c_str(), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
                return nullptr;
            }
#else
            return Internal::g_registry[type](gameObject);
#endif
        }
    }
}
