#pragma once

#include <memory>
#include <unordered_map>
#include <Windows.h>
#include <assert.h>

#pragma region Class Declarations
class Component;
class GameObject;
class ComponentManagerBase;

template <class T>
class ComponentManager;

template<class T>
struct Descriptor;

template<class T>
class Handle;
#pragma endregion
#pragma region Class Definitions
template<>
class Descriptor<GameObject>
{
public:

    size_t managerPosition;


};

enum class ComponentFlags : unsigned char
{
    None = 0x00,
    Active = 0x01,      //State flag
    Destroyed = 0x02,   //State flag
    HasAwaken = 0x04,   //Init flag
    HasStarted = 0x08,  //Init flag
};

DEFINE_ENUM_FLAG_OPERATORS(ComponentFlags);

template<>
struct Descriptor<Component>
{
public:
    ComponentManagerBase* manager;
    size_t managerPosition;
    ComponentFlags flags;
    ComponentFlags nextFlags;
    bool stateChanged;
};

using ComponentPtr = std::shared_ptr<Component>;
using GameObjectHandle = std::weak_ptr<GameObject>;
using ComponentHandle = std::weak_ptr<Component>;

class Component
{
    friend class GameObject;
    friend class ComponentManagerBase;
    template <class T>
    friend class ComponentManager;

    Descriptor<Component> m_descriptor;
    GameObjectHandle m_owningGameObject;
    ComponentHandle m_handle;

public:
    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update() {}
    virtual void OnDestroyed() {}

public:
    void SetActive(bool active);

    bool IsActive() const noexcept { return static_cast<unsigned char>(m_descriptor.flags & ComponentFlags::Active); }

    GameObjectHandle GetGameObject() const noexcept { return m_owningGameObject; }
    virtual ComponentHandle GetHandle() const noexcept { return m_handle; }
};

class ComponentManagerBase
{
    friend class Component;

protected:
    std::vector<ComponentPtr> m_components;
    std::vector<ComponentPtr> m_newComponents;
    std::vector<ComponentHandle> m_stateChanges;
    size_t m_firstInactiveComponent;

public:
    void ProcessStateChanges();
    virtual void Update() = 0;
    virtual ComponentHandle CreateComponent(bool callAwake) = 0;

private:
    void SetComponentActive(const ComponentHandle& component, bool active);
    void DestroyComponent(const ComponentHandle& component);

    void SwapComponentPositions(size_t lh, size_t rh);
};

template<class T>
class ComponentManager final : public ComponentManagerBase
{
public:
    void Update() final
    {
        auto end = m_components.begin() + m_firstInactiveComponent;
        for(auto it = m_components.begin(); it != end; it++)
        {
            static_cast<T*>((*it).get())->T::Update();
        }
    }

    ComponentHandle CreateComponent(bool callAwake) final
    {
        m_newComponents.push_back(std::make_shared<T>());

        //Set the manager to this, and the handle to itself
        m_newComponents.back()->m_descriptor.manager = this;
        m_newComponents.back()->m_handle = m_newComponents.back();
        m_newComponents.back()->SetActive(true);

        if(callAwake)
        {
            static_cast<T*>(m_newComponents.back().get())->T::Awake();
        }

        return m_newComponents.back();
    }
};

class GameObject
{
    friend class Component;

    Descriptor<GameObject> m_descriptor;
    GameObjectHandle m_handle;
    GameObjectHandle m_parent;
    std::vector<std::shared_ptr<GameObject>> m_children;
    std::multimap<std::type_index, ComponentHandle> m_components;

public:
    template<class T>
    std::weak_ptr<T> AddComponent()
    {
        std::shared_ptr<T> ptr;

        m_components.insert({ typeid(T), ptr });

        ptr->m_owningGameObject = m_handle;

        return ptr;
    }
};

#pragma endregion 

#pragma region Class Fuctions
inline void Component::SetActive(bool active)
{
    m_descriptor.manager->SetComponentActive(m_handle, active);
}

inline void ComponentManagerBase::ProcessStateChanges()
{
    //Move all new components into the main component array
    for(ComponentPtr& component : m_newComponents)
    {
        component->m_descriptor.managerPosition = m_components.size();
        m_components.push_back(std::move(component));

        //If a component is to be active
        if((m_components.back()->m_descriptor.nextFlags & ComponentFlags::Active) == ComponentFlags::Active)
        {
            //Modify it's state to pretend it's coming from an inactive state
            m_components.back()->m_descriptor.flags |= ComponentFlags::Active;
            m_components.back()->m_descriptor.flags ^= ComponentFlags::Active;
            m_firstInactiveComponent--;
        }
        else
        {
            //Modify it's state to pretend it's coming from an active state
            m_components.back()->m_descriptor.flags |= ComponentFlags::Active;
            m_firstInactiveComponent++;
        }
    }

    //Process all state changes
    for(const ComponentHandle& component : m_stateChanges)
    {
        ComponentPtr ptr = component.lock();

        //If component is flagged for destroyed
        if((ptr->m_descriptor.nextFlags & ComponentFlags::Destroyed) == ComponentFlags::Destroyed)
        {
            //If they're active, swap them to an inactive state
            if((ptr->m_descriptor.flags & ComponentFlags::Active) == ComponentFlags::Active)
            {
                m_firstInactiveComponent--;
                SwapComponentPositions(m_firstInactiveComponent, ptr->m_descriptor.managerPosition);
            }

            //Swap the component to the end of the vector
            SwapComponentPositions(ptr->m_descriptor.managerPosition, m_components.size() - 1);
            m_components.pop_back();

            ptr->OnDestroyed();
        }
        else if((ptr->m_descriptor.nextFlags & ComponentFlags::Active) == ComponentFlags::Active)
        {
            //Swap first inactive component with the give component
            SwapComponentPositions(m_firstInactiveComponent, ptr->m_descriptor.managerPosition);
            m_firstInactiveComponent++;

            //Our current flags should not be matching
            assert((ptr->m_descriptor.flags ^ ComponentFlags::Active) != (ptr->m_descriptor.nextFlags ^ ComponentFlags::Active));
            ptr->m_descriptor.flags ^= ComponentFlags::Active;

            //If we haven't called start yet, call start
            if((ptr->m_descriptor.flags & ComponentFlags::HasStarted) != ComponentFlags::HasStarted)
            {
                ptr->Awake(); 
                ptr->m_descriptor.flags ^= ComponentFlags::HasStarted;
            }
        }
        else
        {
            //Decrement the first inactive component
            m_firstInactiveComponent--;
            SwapComponentPositions(m_firstInactiveComponent, ptr->m_descriptor.managerPosition);

            //Our current flags should not be matching
            assert((ptr->m_descriptor.flags ^ ComponentFlags::Active) != (ptr->m_descriptor.nextFlags ^ ComponentFlags::Active));
            ptr->m_descriptor.flags ^= ComponentFlags::Active;
        }
        ptr->m_descriptor.stateChanged = false;
    }

    m_stateChanges.clear();
}

inline void ComponentManagerBase::SetComponentActive(const ComponentHandle& component, bool active)
{
    ComponentPtr ptr = component.lock();

    //Check if component is pending destroy
    //If yes return
    if((ptr->m_descriptor.nextFlags & ComponentFlags::Destroyed) == ComponentFlags::Destroyed)
        return;

    ComponentFlags flag = (active) ? ComponentFlags::Active : ComponentFlags::None;
    //Check if next flag's active flag is == active
    //If yes return
    if((ptr->m_descriptor.nextFlags & ComponentFlags::Active) == flag)
        return;

    //Check if old flag's active flag == active 
    //If yes, remove from stateChanges, and return
    //Add component to state changes
    if(!ptr->m_descriptor.stateChanged)
    {
        m_stateChanges.push_back(component);
        ptr->m_descriptor.stateChanged = true;
    }
    ptr->m_descriptor.nextFlags ^= ComponentFlags::Active;
}

inline void ComponentManagerBase::DestroyComponent(const ComponentHandle& component)
{
    ComponentPtr ptr = component.lock();

    //If component has not changed state, change state
    if(!ptr->m_descriptor.stateChanged)
    {
        m_stateChanges.push_back(component);
        ptr->m_descriptor.stateChanged = true;
    }

    ptr->m_descriptor.nextFlags |= ComponentFlags::Destroyed;
}

inline void ComponentManagerBase::SwapComponentPositions(size_t lh, size_t rh)
{
    if(lh == rh)
        return;

    std::swap(m_components[lh], m_components[rh]);
    std::swap(m_components[lh]->m_descriptor.managerPosition, m_components[rh]->m_descriptor.managerPosition);
}

#pragma endregion