#pragma once
#include "../Miscellaneous/ReferencePointer.h"
#include <vector>

namespace CommonsLibrary
{
    class GameObject;
    class ObjectHierarchy
    {
        GameObject* m_gameObject;

        size_t m_hasDestroyedObjects = 0;
        size_t m_hasActiveChangedToInactive = 0;
        size_t m_hasActiveChangedToActive = 0;

        ReferencePointer<GameObject> m_parent;
        ReferencePointer<GameObject> m_nextParent;
        std::vector<ReferencePointer<GameObject>> m_children;

        std::vector<GameObject*> m_activeGameObjects;
        std::vector<GameObject*> m_inactiveGameObjects;

        bool m_hasPreUpdateFlagsSet = false;
        bool m_hasPostUpdateFlagsSet = false;
    public:
        ObjectHierarchy(GameObject* gameObject) : m_gameObject(gameObject) {}

    public:
        void Awake();

        void PreUpdate();
        void Update(float deltaTime);
        void PostUpdate();

    public:
        void RequestParentChange(const ReferencePointer<GameObject>& parent);

        void SetParent(const ReferencePointer<GameObject>& parent);
        void AddChild(ReferencePointer<GameObject> child);
        ReferencePointer<GameObject> RemoveChild(const ReferencePointer<GameObject>& child);

        void SetActive(const ReferencePointer<GameObject>& child, bool active);

        void DestroyGameObject(const ReferencePointer<GameObject>& child);
    public:
        ReferencePointer<GameObject> CreateGameObject();

    public:
        ReferencePointer<GameObject> GetParent() const { return (m_nextParent) ? m_nextParent : m_parent; }
        std::vector<ReferencePointer<GameObject>> GetChildren() const { return m_children; }

    public:
        bool HasPreUpdateFlagsSet() const { return (m_hasActiveChangedToActive + m_hasActiveChangedToInactive) > 0; }
        bool HasPostUpdateFlagsSet() const { return m_hasDestroyedObjects > 0; }

        void SetPreUpdateFlag();
        void SetPostUpdateFlag();

    private:
        void ClearNullChildren();
        void ClearNullObjects(std::vector<GameObject*>& objectVector);

    private:
        void TransferObjects(std::vector<GameObject*>& from, std::vector<GameObject*>& to);

    private:
        void SetParentPreUpdateFlag(bool set);
        void SetParentPostUpdateFlag(bool set);
    };
}