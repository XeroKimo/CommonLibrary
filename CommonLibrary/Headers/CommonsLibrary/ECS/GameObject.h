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
        Transform m_transform;
        ObjectHierarchy m_hierarchy{this};
        ComponentManager m_componentManager{this};

        bool m_active = true;
        bool m_isDestroyed = false;
        bool m_activeChanged = true;
        bool m_hasRequestedActiveChanged = true;

        size_t m_childIndex = 0;

        Scene* m_owningScene = nullptr;

    private:
        GameObject() = default;

    public:
        bool IsActiveInWorld() const
        {
            if(!IsActiveInHeirarchy())
                return false;

            if(GetParent())
                return GetParent()->IsActiveInWorld();

            return true;
        }
        bool IsActiveInHeirarchy() const { return m_active; }
        void SetActive(bool active);

    public:
        void RequestParentChange(const ReferencePointer<GameObject>& parent);
        ReferencePointer<GameObject> GetParent() const;
        std::vector<ReferencePointer<GameObject>> GetChildren() const { return m_hierarchy.GetChildren(); }

    public:
        void SetWorldPosition(Vector3 position) { m_transform.position = position + GetLocalPosition() - GetWorldPosition(); }
        void SetLocalPosition(Vector3 position) { m_transform.position = position; }
        void SetRotation(Quaternion rotation) { m_transform.rotation = rotation; }
        void SetScale(Vector3 scale) { m_transform.scale = scale; }

        Vector3 GetWorldPosition() const
        {
            if(GetParent())
                return GetParent()->GetWorldPosition() + GetLocalPosition();
            return GetLocalPosition();
        }
        Vector3 GetLocalPosition() const { return m_transform.position; }
        Quaternion GetRotation() const { return m_transform.rotation; }
        Vector3 GetScale() const { return m_transform.scale; }

        Matrix4x4 GetLocalTransformMatrix() const { return m_transform.GetMatrix(); }
        Matrix4x4 GetWorldTransformMatrix() const
        {
            if(GetParent())
                return GetParent()->GetWorldTransformMatrix() * GetLocalTransformMatrix();
            return GetLocalTransformMatrix();
        }

        Vector3 Forward() const { return m_transform.Forward(); }
        Vector3 Up() const { return m_transform.Up(); }
        Vector3 Right() const { return m_transform.Right(); }

        Vector3 Backward() const { return -Forward(); }
        Vector3 Down() const { return -Up(); }
        Vector3 Left() const { return -Right(); }

    public:
        template<class Type, std::enable_if_t<std::conjunction_v<std::negation<std::is_same<Type, Component>>, std::is_base_of<Component, Type>>, int> = 0>
        ReferencePointer<Type> AddComponent() { return m_componentManager.CreateComponent<Type>(GetReferencePointer(), SceneLoaded()); }

    private:
        void Awake()
        {
            m_componentManager.Awake();
            m_hierarchy.Awake();
        }
        void ChangeComponentsState() { m_componentManager.ChangeComponentsState(); }
        void TransferParent() { m_hierarchy.TransferParent(); }
        void ChangeChildrenState() { m_hierarchy.ChangeChildrenState(); }
        void Update(float deltaTime)
        {
            m_componentManager.Update(deltaTime);
            m_hierarchy.Update(deltaTime);
        }

    private:
        ReferencePointer<GameObject> CreateChild() { return m_hierarchy.CreateGameObject(); }

    private:
        void SetComponentActive(const ReferencePointer<Component>& component, bool active) { m_componentManager.SetComponentActive(component, active); }
        void DestroyComponent(const ReferencePointer<Component>& component) { m_componentManager.DestroyComponent(component); }
        void CopyComponents(const ComponentManager& other) { m_componentManager.CopyComponents(GetReferencePointer(), other); }

        bool SceneLoaded();

    private:
        ReferencePointer<GameObject> GetRootObject();
        void AddCallChangeComponentsState();
        void AddCallTransferParent();
        void AddCallChangeChildrenState();
    };

    extern void DestroyGameObject(const ReferencePointer<GameObject>& gameObject);
    extern void DestroyGameObject(const ReferencePointer<Component>& component);
    extern void DestroyComponent(const ReferencePointer<Component>& component);
}