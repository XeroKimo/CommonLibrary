#pragma once
#include "CommonsLibrary/Miscellaneous/ReferencePointer.h"
#include "CommonsLibrary/Delegates/Delegate.h"

namespace CommonsLibrary
{
    class GameObject;
    class ComponentMap;
    class World;

#define IsCalledBy(someValue)

    class Component : public ReferencePointerEnableThis<Component>
    {
        friend class ComponentMap;

    public:
        bool active;

    private:
        ReferencePointer<GameObject> m_gameObject;
        bool m_hasStarted;

    public:
        Component(ReferencePointer<GameObject> owningGameObject);
        virtual ~Component() {};


    private:
        IsCalledBy(GameObject::Start())
            void StartComponent();
        IsCalledBy(GameObject::Update())
            void UpdateComponent(float deltaTime);
        IsCalledBy(GameObject::Destroy())
            void DestroyComponent();

    protected:
        virtual void Start() {}
        virtual void Update(float deltaTime) {}
        virtual void OnDestroy() {}

    public:
        ReferencePointer<GameObject> GetGameObject() const { return m_gameObject; }

    public:
        ReferencePointer<GameObject> CreateGameObject();
        World* GetWorld();
    };
}