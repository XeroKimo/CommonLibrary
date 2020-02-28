#pragma once
#include "ReferencePointer.h"

namespace CommonsLibrary
{
    class GameObject;
    class World;

    class Component : public ReferenceFromThis<Component>
    {
        friend class GameObject;
        friend class World;
    private:
        ReferencePointer<GameObject> m_gameObject;
        ReferencePointer<World> m_world;

        bool m_active = true;
        bool m_hasStarted = false;

    public:
        Component(const ReferencePointer<GameObject>& gameObject, const ReferencePointer<World>& world) : m_gameObject(gameObject), m_world(world) {}

    public:
        virtual void Start() {};
        virtual void Update(float deltaTime) {};

        bool IsActive() { return m_active; }
        void SetActive(bool active);

		ReferencePointer<GameObject> GetGameObject();
    };
}