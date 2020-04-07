#pragma once
#include "CommonsLibrary/Miscellaneous/ReferencePointer.h"
#include "CommonsLibrary/Delegates/Delegate.h"
#include "ECSSystem.h"

namespace CommonsLibrary
{
    class GameObject;
    class ComponentMap;
    class ECSSystemManager;
    class Transform;

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
        IsCalledBy(ComponentMap::Start())
            void StartComponent();
        IsCalledBy(ComponentMap::Update())
            void UpdateComponent(float deltaTime);
        IsCalledBy(ComponentMap::OnDestroy())
            void DestroyComponent();

    protected:
        virtual void Start() {}
        virtual void Update(float deltaTime) {}
        virtual void OnDestroy() {}

    public:
        inline ReferencePointer<GameObject> GetGameObject() const { return m_gameObject; }
        ReferencePointer<Transform> GetTransform() const;

    public:
        ReferencePointer<GameObject> CreateGameObject();

        template <class T, std::enable_if_t<std::is_base_of_v<ECSSystem, T>, int> = 0>
        T* GetSystem() const;
    };
}