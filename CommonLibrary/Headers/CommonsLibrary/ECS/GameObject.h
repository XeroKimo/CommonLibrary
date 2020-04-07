#pragma once
#include "Component.h"
#include "CommonsLibrary/StdHelpers/UnorderedMapHelpers.h"
#include "Transform.h"
#include "ComponentMap.h"
#include <typeindex>

namespace CommonsLibrary
{
    class Transform;
    class Scene; 
    class SceneManager;

    class GameObject;

    class GameObject final : public ReferencePointerEnableThis<GameObject>
    {
        friend class Component;
        friend class Transform;
        friend class SceneManager;
        
    protected:
        Scene* m_scene;
        ReferencePointer<Transform> m_transform;
        ComponentMap m_componentMap;

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
        void OnDestroy();

    public:
        template <class Type, std::enable_if_t<std::is_base_of_v<Component, Type>, int> = 0>
        ReferencePointer<Type> AddComponent()
        {
            return m_componentMap.AddComponent<Type>(GetReferencePointer());
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
        void RemoveComponent(const ReferencePointer<Component>& component);

        ReferencePointer<Transform> GetTransform();

        void SetActive(bool active);

        bool GetActiveHeirarchy() const;

        bool GetActiveWorld() const;

        void Destroy();

    private:
        void SetChildrenActiveInWorld();
        bool IsParentActiveInWorld();
        void SetActiveInWorld(bool active);

    private:
        ReferencePointer<GameObject> CreateGameObject();
        
    };
}