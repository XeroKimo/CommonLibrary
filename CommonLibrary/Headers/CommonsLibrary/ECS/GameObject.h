#pragma once
#include "Component.h"
#include "Transform.h"
#include "ComponentManager.h"

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
    public:
        std::string name;

    private:
        ReferencePointer<Transform> m_transform;

        ComponentManager m_componentManager;

        bool m_active = true;
        bool m_isConstructed = true;
        bool m_isActiveWorld = true;
        bool m_isActiveInHeirarchy = true;

    private:
        void Awake();
        void PreUpdate();
        void Update(float deltaTime);
        void PostUpdate();

    public:
        template<class Type, std::enable_if_t<std::conjunction_v<std::negation<std::is_same<Type, Component>>, std::is_base_of<Component, Type>>, int> = 0>
        ReferencePointer<Type> AddComponent() { return m_componentManager.CreateComponent<Type>(GetReferencePointer(), m_isConstructed, SceneLoaded()); }

        ReferencePointer<Transform> GetTransform() const { return m_transform; }
        bool IsActiveWorld() const { return m_isActiveWorld; }
        bool IsActiveInHeirarchy() const { return m_isActiveInHeirarchy; }
    public:
        void SetActive(bool active);

    private:
        void SetActiveWorld(bool active);

    private:
        void SetComponentActive(const ReferencePointer<Component>& component, bool active) { m_componentManager.SetComponentActive(component, active); }
        void DestroyComponent(const ReferencePointer<Component>& component) { m_componentManager.DestroyComponent(component); }

        //TODO: Proper check on scene loaded
        bool SceneLoaded() { return true; }
        void CopyComponents(const ComponentManager& other) { m_componentManager.CopyComponents(GetReferencePointer(), m_isConstructed, SceneLoaded(), other); }

    private:
        static ReferencePointer<GameObject> Construct();
        static ReferencePointer<GameObject> CopyConstruct(const ReferencePointer<GameObject>& other);
    };

    extern void DestroyGameObject(const ReferencePointer<GameObject>& gameObject);
    extern void DestroyGameObject(const ReferencePointer<Component>& component);
    extern void DestroyComponent(const ReferencePointer<Component>& component);
}