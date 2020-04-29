#pragma once
#include "CommonsLibrary/Miscellaneous/ReferencePointer.h"
#include "CommonsLibrary/Delegates/Delegate.h"

#include <unordered_map>
#include <typeindex>


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

    protected:
        virtual void CopyComponent(const Component* const other) = 0;
    };





    class ComponentRegistry
    {
        using CreateCallback = Delegate<ReferencePointer<Component>(const ReferencePointer<GameObject>&)>;
        //static std::unordered_map<std::string, CreateCallback> s_registeredComponents;
        //static std::vector<Component*> s_components;
    public:
        template <class Type>
        static void RegisterComponent()
        {
            std::string name = typeid(Type).name();
            name = name.substr(name.find(' ') + 1);
            GetRegisteredComponents()[name].Bind<CreateComponentCallback<Type>>();
        }

        static ReferencePointer<Component> CreateComponent(std::type_index type, const ReferencePointer<GameObject>& gameObject);
        static ReferencePointer<Component> CreateComponent(std::string name, const ReferencePointer<GameObject>& gameObject);

        static void RegisteredStaticComponent(Component* component, void** staticPointer);
        static void ClearRegisteredStatics();
        static bool IsComponentRegistered(std::type_index type);
    private:
        template <class Type>
        static ReferencePointer<Component> CreateComponentCallback(const ReferencePointer<GameObject>& gameObject)
        {
            return MakeReference<Type>(gameObject);
        }

        static std::unordered_map<std::string, CreateCallback>& GetRegisteredComponents();
        static std::vector<std::pair<Component*, void**>>& GetStaticComponents();
    };





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