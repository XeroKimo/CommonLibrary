#pragma once
#include "../Miscellaneous/ReferencePointer.h"
#include <vector>

namespace CommonsLibrary
{
    class GameObject;
    class ObjectHierarchy
    {
        GameObject* m_gameObject;
        ReferencePointer<GameObject> m_parent;
        ReferencePointer<GameObject> m_nextParent;
        std::vector<ReferencePointer<GameObject>> m_children;

        std::vector<GameObject*> m_activeGameObjects;
        std::vector<GameObject*> m_inactiveGameObjects;

        bool m_hasPreUpdateFlagsSet;
        bool m_hasPostUpdateFlagsSet;
    public:
        ObjectHierarchy(GameObject* gameObject) : m_gameObject(gameObject) {}

    public:
        void Awake();
        void BeginTransfers();

        void PreUpdate();
        void Update(float deltaTime);
        void PostUpdate();

    public:
        void RequestParentChange(const ReferencePointer<GameObject>& parent);

        void SetParent(const ReferencePointer<GameObject>& parent);
        void AddChild(ReferencePointer<GameObject> child);
        ReferencePointer<GameObject> RemoveChild(const ReferencePointer<GameObject>& child);

    public:
        ReferencePointer<GameObject> CreateGameObject();

    public:
        ReferencePointer<GameObject> GetParent() const { return m_parent; }
        std::vector<ReferencePointer<GameObject>> GetChildren() const { return m_children; }


    private:
        void ClearNullChildren();
        void ClearNullObjects(std::vector<GameObject*>& objectVector);
    };
}