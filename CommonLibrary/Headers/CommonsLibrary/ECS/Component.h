#pragma once
#include "ComponentRegistry.h"


namespace CommonsLibrary
{
    class GameObject;

    class Component : public ReferencePointerEnableThis<Component>
    {
        friend class GameObject;
        friend class ComponentRegistry;
        friend class ComponentManager;

    private:
        bool m_active = true;
        bool m_isDestroyed = false;
        bool m_hasStarted = false;
        bool m_activeChanged = true;

        ReferencePointer<GameObject> m_gameObject;

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

    public:
        void SetActive(bool active);

        bool GetActive() const { return m_active; }
        ReferencePointer<GameObject> GetGameObject() const { return m_gameObject; }
    };



    //inside .h
#define ECS_COMPONENT_SETUP(ComponentType, BaseComponentType) \
public: \
    ComponentType(const ReferencePointer<GameObject>& owningGameObject) : \
        BaseComponentType(owningGameObject) \
    {} \
public: \
    using Base = BaseComponentType; \
    using Super = BaseComponentType; \
    static ComponentType* s_internalComponent; \
protected: \
    ComponentType() : BaseComponentType() \
    {   \
        if(!ComponentRegistry::IsComponentRegistered(typeid(ComponentType)))    \
            InternalReigsterComponent(); \
    }  \
private: \
    void InternalReigsterComponent();   \




    //Inside cpps
#define ECS_COMPONENT_REGISTER_IN_NAMESPACE(ComponentType, NameSpace)   \
    NameSpace::ComponentType* NameSpace::ComponentType::s_internalComponent = new NameSpace::ComponentType();    \
    \
    void NameSpace::ComponentType::InternalReigsterComponent()   \
    {   \
            ComponentRegistry::RegisterComponent<ComponentType>();    \
            ComponentRegistry::RegisteredStaticComponent(this, reinterpret_cast<void**>(&s_internalComponent));    \
    }   

#define ECS_COMPONENT_REGISTER(ComponentType)  \
    ComponentType* ComponentType::s_internalComponent = new ComponentType();    \
    \
    void ComponentType::InternalReigsterComponent()   \
    {   \
            ComponentRegistry::RegisterComponent<ComponentType>();    \
            ComponentRegistry::RegisteredStaticComponent(this, reinterpret_cast<void**>(&s_internalComponent));    \
    }   
}