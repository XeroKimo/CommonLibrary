#pragma once
#include "CommonsLibrary/Miscellaneous/ReferencePointer.h"
#include "CommonsLibrary/Delegates/Delegate.h"

namespace CommonsLibrary
{
    class GameObject;
    __interface IGameObject;
    struct UpdateableComponents;

    class World;
    class Scene;


    class Component : public ReferenceFromThis<Component>
    {
        friend struct UpdateableComponents;
        friend class GameObject;
    private:
        bool m_active = true;
        bool m_hasStarted = false;

    private:
        ReferencePointer<IGameObject> m_gameObject;

    public:
        Component() = delete;
        Component(const ReferencePointer<IGameObject>& _gameObject);


        bool IsActive() { return m_active; }
        void SetActive(bool active);

        ReferencePointer<IGameObject> GetGameObject();
        Scene* GetScene();
        World* GetWorld();

    public:
        virtual void Start() {};
        virtual void Update(float deltaTime) {};
        virtual void OnDestroy() {};

    };
}