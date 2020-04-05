#include "CommonsLibrary/ECS/ComponentRegistry.h"
#include "CommonsLibrary/ECS/World.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/DebugTools/Assert.h"

#include <locale>
#include <codecvt>
#include <string>

#include <Windows.h>

namespace CommonsLibrary
{
    ReferencePointer<Component> ComponentRegistry::Create(const std::type_index& type, const ReferencePointer<GameObject>& gameObject)
    {
        return Create(type.name(), gameObject);
    }
    ReferencePointer<Component> ComponentRegistry::Create(const std::string& type, const ReferencePointer<GameObject>& gameObject)
    {
#if _DEBUG
        if (KeyExists(m_registry, type))
        {
            return m_registry[type](gameObject);
        }
        else
        {
            std::unique_ptr<wchar_t[]> m_componentName = std::make_unique<wchar_t[]>(type.size());

            MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, type.c_str(), type.size() - 1, m_componentName.get(), type.size() - 1);

            std::wstring errorMessage = m_componentName.get();
            errorMessage += L" is not reigstered";
            _wassert(errorMessage.c_str(), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
            return nullptr;
        }
#else
        return Internal::g_registry[type](gameObject);
#endif
    }
}
