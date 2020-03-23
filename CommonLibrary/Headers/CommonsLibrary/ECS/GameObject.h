#pragma once
#include "Component.h"
#include "CommonsLibrary/StdHelpers/UnorderedMapHelpers.h"
#include "Transform.h"
#include "ComponentMap.h"
#include "UpdateableComponents.h"
#include <typeindex>
#include "IGameObject.h"

namespace CommonsLibrary
{
    class Transform;
    class Scene;

    class GameObject;

    class GameObject final : public IGameObject, public ReferenceFromThis<GameObject>
    {
        friend class Component;
        friend class Transform;
        
    protected:
        Scene* m_scene;
        ReferencePointer<Transform> m_transform;
        ComponentMap m_componentMap;
        UpdateableComponents m_updateableComponents;

        bool m_activeInWorld;
        bool m_activeInHeirarchy;

        bool m_hasComponentToStart;
        bool m_hasComponentToRemove;
        bool m_isDestroyed;
    public:
        std::string name;

    public:
        GameObject(Scene* const scene);

    public:
        void Start();
        void Update(float deltaTime);
        void CleanUpComponents();
        void OnDestroy();

    public:
        template <class Type, class Enable = std::enable_if_t<std::is_base_of_v<Component, Type>>>
        ReferencePointer<Type> AddComponent()
        {
            ReferencePointer<Type> component = m_componentMap.AddComponent<Type>(GetReferencePointer());
            m_updateableComponents.AddComponent(component.Get());
            return component;
        }

        template <class Type>
        ReferencePointer<Type> GetComponent()
        {
            return m_componentMap.GetComponent<Type>();
        }

        template <class Type>
        std::vector<ReferencePointer<Type>> GetComponents()
        {
            return m_componentMap.GetComponents<Type>();
        }

    public:
        // Inherited via IGameObject
        virtual ReferencePointer<Component> AddComponent(const std::type_index& key) override;

        virtual void RemoveComponent(const ReferencePointer<Component>& component) override;

        virtual ReferencePointer<Component> GetComponent(const std::type_index& key) override;

        virtual std::vector<ReferencePointer<Component>> GetComponents(const std::type_index& key) override;

        virtual ReferencePointer<Transform> GetTransform() override;

        virtual void SetName(std::string_view name) override;

        virtual std::string GetName() const override;

        virtual void SetActive(bool active) override;

        virtual bool GetActiveHeirarchy() const override;

        virtual bool GetActiveWorld() const override;

        virtual void Destroy() override;

    private:
        void SetComponentActive(const ReferencePointer<Component>& component);

    private:
        void AddGameObjectToStart();
        void AddGameObjectToCleanUp();

        void SetChildrenActiveInWorld();
        bool IsParentActiveInWorld();
        void SetActiveInWorld(bool active);
    };
}