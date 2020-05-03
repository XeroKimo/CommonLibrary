#pragma once
#include "Component.h"
#include "Transform.h"
#include "ComponentManager.h"
#include "ObjectHierarchy.h"

#include <string>
#include <typeindex>

namespace CommonsLibrary
{
    class Scene;
    class GameObject : public ReferencePointerEnableThis<GameObject>
    {
        friend void DestroyGameObject(const ReferencePointer<GameObject>& gameObject);
        friend void DestroyGameObject(const ReferencePointer<Component>& component);
        friend void DestroyComponent(const ReferencePointer<Component>& component);

        friend class Component;
        friend class Transform;
        friend class ComponentManager;
        friend class ObjectHierarchy;
        friend class Scene;
    public:
        std::string name;

    private:
        Transform m_transform{this};
        ObjectHierarchy m_hierarchy{this};
        ComponentManager m_componentManager{this};

        bool m_active = true;
        bool m_isDestroyed = false;
        bool m_activeChanged = true;

        size_t m_childIndex = 0;

        Scene* m_owningScene = nullptr;

    private:
        GameObject() = default;

    private:
        void PreAwake();
        void Awake()
        {
            m_componentManager.Awake();
            m_hierarchy.Awake();
        }
        void StartComponents() { m_componentManager.Start(); }
        void StartHierarchy() { m_hierarchy.Start(); }
        void PostStart() { m_hierarchy.PostStart(); }
        void Update(float deltaTime)
        {
            m_componentManager.Update(deltaTime);
            m_hierarchy.Update(deltaTime);
        }

    public:
        template<class Type, std::enable_if_t<std::conjunction_v<std::negation<std::is_same<Type, Component>>, std::is_base_of<Component, Type>>, int> = 0>
        ReferencePointer<Type> AddComponent() { return m_componentManager.CreateComponent<Type>(GetReferencePointer(), SceneLoaded()); }

        Transform& GetTransform() { return m_transform; }
        bool IsActiveInWorld() const { return (m_hierarchy.GetParent()) ? m_hierarchy.GetParent()->IsActiveInHeirarchy() && IsActiveInHeirarchy() : IsActiveInHeirarchy(); }
        bool IsActiveInHeirarchy() const { return m_active; }
        void SetActive(bool active);

    public:
        ReferencePointer<GameObject> CreateChild() { return m_hierarchy.CreateGameObject(!SceneLoaded()); }

    public:
        void SetParent(const ReferencePointer<GameObject>& parent);
        ReferencePointer<GameObject> GetParent() const;
        std::vector<ReferencePointer<GameObject>> GetChildren() const { return m_hierarchy.GetChildren(); }


    private:
        void SetComponentActive(const ReferencePointer<Component>& component, bool active) { m_componentManager.SetComponentActive(component, active); }
        void DestroyComponent(const ReferencePointer<Component>& component) { m_componentManager.DestroyComponent(component); }
        void CopyComponents(const ComponentManager& other) { m_componentManager.CopyComponents(GetReferencePointer(), other); }

        bool SceneLoaded();

    private:
        ReferencePointer<GameObject> GetRootObject();
        void AddCallStartOnComponents();
        void AddCallStartOnHierarchy();
        void AddPostStartCall();

    public:
        //static ReferencePointer<GameObject> Construct();
        //static ReferencePointer<GameObject> CopyConstruct(const ReferencePointer<GameObject>& other);
    };

    extern void DestroyGameObject(const ReferencePointer<GameObject>& gameObject);
    extern void DestroyGameObject(const ReferencePointer<Component>& component);
    extern void DestroyComponent(const ReferencePointer<Component>& component);
}