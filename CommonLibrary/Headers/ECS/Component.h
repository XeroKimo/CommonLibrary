#pragma once
#include "ReferencePointer.h"

namespace CommonsLibrary
{
    class GameObject;
    class World;

    class Component : public ReferenceFromThis<Component>
    {
    private:
        ReferencePointer<GameObject> m_gameObject;
        ReferencePointer<World> m_world;

        bool m_active;
        bool m_hasStarted;

    public:
        Component() = default;
        //Component(const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world) : m_gameObject(gameObject), m_world(world) {}

    public:
        virtual void Start() = 0;
        virtual void Update(float deltaTime) = 0;

        bool IsActive() { return m_active; }
        void SetActive(bool active);

        ReferencePointer<GameObject> GetGameObject() { return m_gameObject; }
    };
}