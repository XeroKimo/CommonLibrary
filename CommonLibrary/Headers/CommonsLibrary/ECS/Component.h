#pragma once
#include "ComponentRegistry.h"


namespace CommonsLibrary
{
    class GameObject;

    class Component : public ReferenceViewEnableThis<Component>
    {
        friend class GameObject;
        friend class ComponentRegistry;
        friend class ComponentManager;

    private:
        bool m_active = true;
        bool m_isDestroyed = false;
        bool m_hasStarted = false;
        bool m_activeChanged = true;
        bool m_hasRequestedActiveChanged = true;

        ReferencePointer<GameObject> m_gameObject;

        size_t m_componentIndex = 0;

    protected:
        Component() = default;

    public:
        Component(const ReferencePointer<GameObject>& owningGameObject);
        virtual ~Component() = default;

    protected:
        virtual void Awake() = 0;
        virtual void Start() {}
        virtual void Update(float deltaTime) {}
        virtual void OnDestroyed() {}

        virtual void CopyComponent(const Component* const other);

    public:
        void SetActive(bool active);

        bool GetActive() const { return m_active; }
        ReferencePointer<GameObject> GetGameObject() const { return m_gameObject; }
    };

    class EmptyBase
    {
    };

    template<class T>
    class SelfRegisterComponent : EmptyBase
    {
        static SelfRegisterComponent<T>* s_internal;

    public:
        SelfRegisterComponent()
        {
            if(!ComponentRegistry::IsComponentRegistered(typeid(T)))
                ComponentRegistry::RegisterComponent<T>();
        }
    };

    //inside .h
#define ECS_COMPONENT_SETUP(ComponentType, BaseComponentType) \
public: \
    ComponentType(const CommonsLibrary::ReferencePointer<CommonsLibrary::GameObject>& owningGameObject) : \
        BaseComponentType(owningGameObject) \
    {} \
public: \
    using Base = BaseComponentType; \
    using Super = BaseComponentType;


    //Inside cpps
#define ECS_COMPONENT_REGISTER_IN_NAMESPACE(ComponentType, NameSpace)   \
    CommonsLibrary::SelfRegisterComponent<NameSpace::ComponentType>* CommonsLibrary::SelfRegisterComponent<NameSpace::ComponentType>::s_internal = new SelfRegisterComponent<NameSpace::ComponentType>();

#define ECS_COMPONENT_REGISTER(ComponentType)  \
    CommonsLibrary::SelfRegisterComponent<ComponentType>* CommonsLibrary::SelfRegisterComponent<ComponentType>::s_internal = new SelfRegisterComponent<ComponentType>();
}