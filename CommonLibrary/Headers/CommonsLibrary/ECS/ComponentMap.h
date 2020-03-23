#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "CommonsLibrary/StdHelpers/UnorderedMapHelpers.h"

namespace CommonsLibrary
{
    class GameObject;
    class ComponentMap
    {
        std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>> m_componentMap;
    public:
        ComponentMap() = default;
        ComponentMap(const ComponentMap& other) = delete;
        ComponentMap(ComponentMap&& other) = delete;

        void operator=(const ComponentMap& other) = delete;
        void operator=(ComponentMap&& other) = delete;

    public:
        template <class Type, class Enable = std::enable_if_t<std::is_base_of_v<Component, Type>>>
        ReferencePointer<Type> AddComponent(const ReferencePointer<GameObject>& gameObject)
        {
            static_assert(std::is_convertible<std::remove_cv_t<Type>*, typename Component::RefThis*>::value, "Multiple inheritance of type Component is not allowed");
            std::type_index key(typeid(Type));

            auto& componentVector = m_componentMap[key];
            //componentVector.push_back(ReferencePointerStaticCast<Type>(ComponentRegistry::Create(key, gameObject)));
            return nullptr;//componentVector.back();
        }

        template <class Type>
        ReferencePointer<Type> GetComponent()
        {
            std::type_index key(typeid(Type));
            if (KeyExists(m_componentMap, key))
            {
                return ReferencePointerStaticCast<Type>(m_componentMap[key][0]);
            }
            else
            {
                ReferencePointer<Type> castSuccessful;
                for (const auto& componentPair : m_componentMap)
                {
                    castSuccessful = ReferencePointerDynamicCast<Type>(componentPair.second[0]);
                    if (castSuccessful)
                        return castSuccessful;
                }
                return nullptr;
            }
        }

        template <class Type>
        std::vector<ReferencePointer<Type>> GetComponents()
        {
            std::type_index key(typeid(Type));
            std::vector<Type> components;

            for (const auto& componentPair : m_componentMap)
            {
                if (componentPair.first == key)
                {
                    StaticCopyComponents<Type>(componentPair.second, components);
                }
                else
                {
                    ReferencePointer<Type> castSuccessful = ReferencePointerDynamicCast<Type>(componentPair.second[0]);
                    if (castSuccessful)
                    {
                        StaticCopyComponents<Type>(componentPair.second, components);
                    }
                }
            }

            return components;
        }

        ReferencePointer<Component> RemoveComponent(const ReferencePointer<Component>& component);

    private:
        template <class Type>
        void StaticCopyComponents(const std::vector<Component>& components, std::vector<Type>& outputVector)
        {
            for (ReferencePointer<Component> component : components)
            {
                outputVector.push_back(ReferencePointerStaticCast<Type>(component));
            }
        }
    };
}