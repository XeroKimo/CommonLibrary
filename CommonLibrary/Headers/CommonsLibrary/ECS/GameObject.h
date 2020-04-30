#pragma once
#include "Component.h"
#include "Transform.h"
#include "ComponentManager.h"
#include "ObjectHierarchy.h"

#include <string>
#include <typeindex>

namespace CommonsLibrary
{
    class GameObject : public ReferencePointerEnableThis<GameObject>
    {
        friend void DestroyGameObject(const ReferencePointer<GameObject>& gameObject);
        friend void DestroyGameObject(const ReferencePointer<Component>& component);
        friend void DestroyComponent(const ReferencePointer<Component>& component);

        friend class Component;
        friend class Transform;
        friend class GameObjectManager;
        friend class ObjectHierarchy;
    public:
        std::string name;

    private:
        ReferencePointer<Transform> m_transform;
        ObjectHierarchy m_hierarchy{this};
        ComponentManager m_componentManager;

        bool m_isActiveWorld = true;
        bool m_isActiveInHeirarchy = true;
        bool m_isDestroyed = false;

    public:
        template<class Type, std::enable_if_t<std::conjunction_v<std::negation<std::is_same<Type, Component>>, std::is_base_of<Component, Type>>, int> = 0>
        ReferencePointer<Type> AddComponent() { return m_componentManager.CreateComponent<Type>(GetReferencePointer(), SceneLoaded()); }

        ReferencePointer<Transform> GetTransform() const { return m_transform; }
        bool IsActiveInWorld() const { return m_isActiveWorld; }
        bool IsActiveInHeirarchy() const { return m_isActiveInHeirarchy; }
        void SetActive(bool active);

    private:
        void Awake();
        void PreUpdate();
        void Update(float deltaTime);
        void PostUpdate();

    private:
        void SetActiveWorld(bool active);

    private:
        void SetComponentActive(const ReferencePointer<Component>& component, bool active) { m_componentManager.SetComponentActive(component, active); }
        void DestroyComponent(const ReferencePointer<Component>& component) { m_componentManager.DestroyComponent(component); }
        void CopyComponents(const ComponentManager& other) { m_componentManager.CopyComponents(GetReferencePointer(), other); }

        bool HasPreUpdateFlagsSet() const { return m_componentManager.HasPreUpdateFlagsSet(); }
        bool HasPostUpdateFlagsSet() const { return m_componentManager.HasPostUpdateFlagsSet(); }

        //TODO: Proper check on scene loaded
        bool SceneLoaded() { return true; }

    private:
        void AddChild(ReferencePointer<GameObject> child) { m_hierarchy.AddChild(std::move(child)); }
        ReferencePointer<GameObject> RemoveChild(const ReferencePointer<GameObject>& child) { return m_hierarchy.RemoveChild(child); }

    public:
        ReferencePointer<GameObject> GetParent() const { return m_hierarchy.GetParent(); }
        std::vector<ReferencePointer<GameObject>> GetChildren() const { return m_hierarchy.GetChildren(); }

    private:
        static ReferencePointer<GameObject> Construct();
        static ReferencePointer<GameObject> CopyConstruct(const ReferencePointer<GameObject>& other);
    };

    extern void DestroyGameObject(const ReferencePointer<GameObject>& gameObject);
    extern void DestroyGameObject(const ReferencePointer<Component>& component);
    extern void DestroyComponent(const ReferencePointer<Component>& component);
}