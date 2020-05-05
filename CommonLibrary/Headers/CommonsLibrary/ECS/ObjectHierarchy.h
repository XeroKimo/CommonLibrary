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

        ReferencePointer<GameObject> m_parent;
        ReferencePointer<GameObject> m_nextParent;

        std::priority_queue<size_t> m_destroyedObjectIndices;
        std::vector<ReferencePointer<GameObject>> m_activeChangedObjects;

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