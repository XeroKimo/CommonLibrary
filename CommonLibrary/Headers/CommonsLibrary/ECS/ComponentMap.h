#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "CommonsLibrary/StdHelpers/UnorderedMapHelpers.h"

namespace CommonsLibrary
{
    class GameObject;
    class ComponentMap
    {
        std::vector<Component*> m_updateComponents;
        std::vector<ReferencePointer<Component>> m_componentsToDestroy;
        std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>> m_componentMap;
    public:
        ComponentMap() = default;
        ComponentMap(const ComponentMap& other) = delete;
        ComponentMap(ComponentMap&& other) = delete;

        void operator=(const ComponentMap& other) = delete;
        void operator=(ComponentMap&& other) = delete;

    public:
        void CleanUp();

    public:
        void Start();
        void Update(float deltaTime);
        void OnDestroy();

    public:
        ReferencePointer<Component> AddComponent(const ReferencePointer<GameObject>& gameObject, const std::type_index& key);
        ReferencePointer<Component> GetComponent(const std::type_index& key);
        std::vector<ReferencePointer<Component>> GetComponents(const std::type_index& key);

    public:
        template <class Type, std::enable_if_t<std::is_base_of_v<Component, Type>, int> = 0>
        ReferencePointer<Type> AddComponent(const ReferencePointer<GameObject>& gameObject)
        {
            static_assert(std::is_convertible<std::remove_cv_t<Type>*, typename Component::RefThis*>::value, "Multiple inheritance of type Component is not allowed");
            return ReferencePointerStaticCast<Type>(AddComponent(gameObject, typeid(Type)));
        }

        template <class Type>
        ReferencePointer<Type> GetComponent()
        {
            std::type_index key(typeid(Type));
            ReferencePointer<Component> component = GetComponent(key);
            if (component)
            {
                return ReferencePointerStaticCast<Type>(component);
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

        void RemoveComponent(const ReferencePointer<Component>& component);

    public:
        void DestroyAllComponents();

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