#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "UnorderedMapHelpers.h"
#include "Transform.h"
#include <typeindex>


namespace CommonsLibrary
{
    class Transform;
    class GameObject final : public ReferenceFromThis<GameObject>
    {
        friend class Component;
        friend class World;
    private:
        std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>> m_activeComponents;
        std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>> m_inactiveComponents;
        ReferencePointer<Transform> m_transform;
        ReferencePointer<World> m_world;

        bool m_active;

    public:
        std::string name;
    public:
		GameObject() = default;
        GameObject(const ReferencePointer<World>& world);

    public:
        void Start();
        void Update(float deltaTime);

    public:
        template<class Type, class = std::enable_if_t<std::is_base_of_v<Component, Type>>>
        ReferencePointer<Type> AddComponent()
        {
            std::type_index key(typeid(Type));

            auto& components = m_activeComponents[key];
            components.push_back(ComponentRegistry::Create(key, GetReferencePointer(), m_world));
            return ReferencePointerStaticCast<Type>(components.back());
        }

        void RemoveComponent(ReferencePointer<Component> component);

        template<class Type>
        ReferencePointer<Type> GetComponent() const
        {
            std::type_index key(typeid(Type));
            ReferencePointer<Type> component = GetComponent<Type>(m_activeComponents, key);
            return (component) ? component : GetComponent<Type>(m_inactiveComponents, key);
        }

        template<class Type>
        std::vector<Type> GetComponents() const
        {
            std::type_index key(typeid(Type));
            std::vector<Type> components;
            GetComponents<Type>(m_activeComponents, key, components);
            GetComponents<Type>(m_inactiveComponents, key, components);
            return components;
        }

		ReferencePointer<Transform> GetTransform() { return m_transform; }
    private:
        void SetComponentActive(const ReferencePointer<Component>& component, bool active);

        template<class Type>
        ReferencePointer<Type> GetComponent(std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>> componentMap, const std::type_index& key) const
        {
            if (KeyExists(componentMap, key))
            {
                return ReferencePointerStaticCast<Type>(componentMap[key][0]);
            }
            else
            {
                ReferencePointer<Type> type;
                for (const auto& componentPair : componentMap)
                {
                    type = ReferencePointerDynamicCast<Type>(componentPair.second[0]);
                    if (type)
                        return type;
                }
                return nullptr;
            }
        }

        template<class Type>
        void GetComponents(std::unordered_map<std::type_index, std::vector<ReferencePointer<Component>>> componentMap, const std::type_index& key, std::vector<Type>& components) const
        {
            for (const auto& componentPair : componentMap)
            {
                if (componentPair.first == key)
                {
                    for (ReferencePointer<Component> component : componentPair.second)
                    {
                        components.push_back(ReferencePointerStaticCast<Type>(component));
                    }
                }
                else
                {
                    ReferencePointer<Type> castComponent = ReferencePointerDynamicCast<Type>(componentPair.second[0]);
                    if (castComponent)
                    {
                        components.push_back(castComponent);
                        for (int i = 1; i < componentPair.second.size(); i++)
                        {
                            components.push_back(ReferencePointerStaticCast<Type>(componentPair.second[i]));
                        }
                    }
                }
            }
        }
    };
}