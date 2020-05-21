#pragma once
#include "../Miscellaneous/ReferencePointer.h"
#include <vector>
#include <queue>

namespace CommonsLibrary
{
    class GameObject;

    class ObjectHierarchy
    {
        GameObject* m_gameObject;

        ReferenceView<GameObject> m_parent;
        ReferenceView<GameObject> m_nextParent;

        std::priority_queue<size_t> m_destroyedObjectIndices;
        std::vector<ReferenceView<GameObject>> m_activeChangedObjects;

        size_t m_firstInactiveObjectIndex = 0;

        std::vector<ReferencePointer<GameObject>> m_children;

        bool m_toldSceneToTransferParent = false;
        bool m_toldSceneToChangeChildrenState = false;
    public:
        ObjectHierarchy(GameObject* gameObject) : m_gameObject(gameObject) {}

    public:

        void Awake();
        void TransferParent();
        void ChangeChildrenState();
        void Update(float deltaTime);

    public:
        void RequestParentChange(const ReferenceView<GameObject>& parent);

        void SetParent(const ReferenceView<GameObject>& parent);
        void AddChild(ReferencePointer<GameObject> child);
        ReferencePointer<GameObject> RemoveChild(const ReferenceView<GameObject>& child);

        void SetActive(const ReferenceView<GameObject>& child, bool active);
        void DestroyGameObject(const ReferenceView<GameObject>& child);
    public:
        ReferenceView<GameObject> CreateGameObject();

    public:
        ReferenceView<GameObject> GetParent() const { return m_parent; }
        std::vector<ReferenceView<GameObject>> GetChildren() const
        {
            std::vector<ReferenceView<GameObject>> children;
            std::copy(m_children.begin(), m_children.end(), std::back_inserter(children));
            return children;
        }

    private:
        void AddToStartCall();
        void AddToPostStartCall();

    private:        
        void ClearDestroyedChildren();
        void SwapChildrenStates();
        void ChangeParent();

        void SwapObjectActive(size_t objectIndex);
        void SwapObject(size_t lh, size_t rh);

        void RecountIndicesStarting(size_t startIndex);
    };
}