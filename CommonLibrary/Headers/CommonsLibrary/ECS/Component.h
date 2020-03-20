#pragma once
#include "CommonsLibrary/Miscellaneous/ReferencePointer.h"

namespace CommonsLibrary
{
    class GameObject;
    class World;
    class Scene;
    class Component : public ReferenceFromThis<Component>
    {
        friend class GameObject;

    private:
        bool m_active = true;
        bool m_hasStarted = false;

    private:
        ReferencePointer<GameObject> m_gameObject;

    public:
        Component() = delete;
        Component(const ReferencePointer<GameObject>& _gameObject);


        bool IsActive() { return m_active; }
        void SetActive(bool active);

        ReferencePointer<GameObject> GetGameObject();
        Scene* GetScene();
        World* GetWorld();

    protected:
        virtual void Start() {};
        virtual void Update(float deltaTime) {};
        virtual void OnDestroy() {};

    };
}