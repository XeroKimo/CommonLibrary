#pragma once
#include "../Miscellaneous/ReferencePointer.h"
#include <vector>

namespace CommonsLibrary
{
    class GameObject;
    class ObjectHierarchy
    {
        GameObject* m_gameObject;

        size_t m_countOfDestroyedObjects = 0;
        size_t m_countOfChangedToInactive = 0;
        size_t m_countOfChangedToActive = 0;

        size_t m_lastActiveChildIndex = 0;

        ReferencePointer<GameObject> m_parent;
        ReferencePointer<GameObject> m_nextParent;

        std::vector<ReferencePointer<GameObject>> m_children;

        bool m_hasPreUpdateFlagsSet = false;
        bool m_hasPostUpdateFlagsSet = false;
        bool m_childHasPreFlagSet = false;
        bool m_childHasPostFlagSet = false;
    public:
        ObjectHierarchy(GameObject* gameObject) : m_gameObject(gameObject) {}

    public:
        void PreAwake();
        void Awake();

        void PreUpdate();
        void Start();
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
        ReferencePointer<GameObject> GetParent() const { return m_parent; }
        std::vector<ReferencePointer<GameObject>> GetChildren() const { return m_children; }

    public:
        bool HasPreUpdateFlagsSet() const { return (m_countOfChangedToActive + m_countOfChangedToInactive) > 0; }
        bool HasPostUpdateFlagsSet() const { return m_countOfDestroyedObjects > 0 || m_nextParent; }

        void SetPreUpdateFlag();
        void SetPostUpdateFlag();

    private:
        void TransferObjects(std::vector<GameObject*>& from, std::vector<GameObject*>& to);

    private:
        void SetParentPreUpdateFlag(bool set);
        void SetParentPostUpdateFlag(bool set);

        void IncreaseIndicesAfter(size_t index, size_t amount);
        void ReduceIndicesAfter(size_t index, size_t amount);
    };
}