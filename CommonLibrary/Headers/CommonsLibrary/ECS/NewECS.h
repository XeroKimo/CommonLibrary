#pragma once

#include <memory>
#include <unordered_map>
#include <Windows.h>

class Component;
class GameObject;
class ComponentManagerBase;

template <class T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
class ComponentManager;

template<class T>
class Descriptor;

template<class T, std::enable_if_t<std::disjunction_v<std::is_base_of<Component, T>, std::is_same<T, GameObject>>, int> = 0>
class is_component
{
public:
    using type = Component;

    static constexpr bool value = true;
};

template<class T>
using is_component_t = typename is_component<T>::type;

template<>
class Descriptor<GameObject>
{
public:
    GameObject* gameObject;

public:
    GameObject* Get()
    {
        return gameObject;
    }
};

enum class ComponentFlags : unsigned char
{
    None = 0x00,
    Active = 0x01,
    Destroyed = 0x02,
    HasAwaken = 0x04,
    HasStarted = 0x08
};

DEFINE_ENUM_FLAG_OPERATORS(ComponentFlags);

template<>
class Descriptor<Component>
{

public:
    ComponentManagerBase* manager;
    Component* component;
    size_t managerPosition;
    ComponentFlags flags;

public:
    Component* Get()
    {
        return component;
    }
};

template<class T>
class Handle
{
    using DescriptorType = is_component_t<T>;
    using HandleType = T;
    using DescriptorPtr = std::shared_ptr<Descriptor<DescriptorType>>;

    template<class Derived>
    friend class Handle;

    friend class Component;
    friend class ::ComponentManagerBase;

    template<class To, class From>
    friend Handle<To> HandleCast(Handle<From> other);

    DescriptorPtr m_descriptor;

    //Change this to private, public for testing purposes
public:
    Handle(const DescriptorPtr& descriptor) : m_descriptor(descriptor) {}

public:
    Handle() = default;
    Handle(std::nullptr_t) : m_descriptor() { }
    Handle(const Handle& other) : m_descriptor(other.m_descriptor) {}

    template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, T*>, int> = 0>
    Handle(const Handle<Derived>& other) : m_descriptor(other.m_descriptor) {}

    Handle(Handle&& other) noexcept : m_descriptor(std::move(other.m_descriptor)) {}

    template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, T*>, int> = 0>
    Handle(Handle<Derived>&& other) : m_descriptor(std::move(other.m_descriptor)) {}

    Handle operator=(const Handle& other) 
    {
        m_descriptor = other.m_descriptor; 
        return *this;
    }

    template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, T*>, int> = 0>
    Handle operator=(const Handle<Derived>& other)
    {
        m_descriptor = other.m_descriptor;
        return *this;
    }

    Handle& operator=(const Handle&& other) noexcept
    { 
        m_descriptor = other.m_descriptor; 
        return *this;
    }

    template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, T*>, int> = 0>
    Handle& operator=(Handle<Derived>&& other) noexcept
    {
        m_descriptor = other.m_descriptor;
        return *this;
    }

    bool operator==(const Handle& other) const
    {
        return m_descriptor == other.m_descriptor;
    }

    template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, T*>, int> = 0>
    bool operator==(const Handle<Derived>& other) const
    {
        return m_descriptor == other.m_descriptor;
    }
    
    bool operator!=(const Handle& other) const
    {
        return m_descriptor != other.m_descriptor;
    }

    template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, T*>, int> = 0>
    bool operator!=(const Handle<Derived>& other) const
    {
        return m_descriptor != other.m_descriptor;
    }

    HandleType& operator*() const
    {
        return *static_cast<HandleType*>(m_descriptor->Get());
    }

    HandleType* operator->() const
    {
        return static_cast<HandleType*>(m_descriptor->Get());
    }

    HandleType* Get() const
    {
        if(!m_descriptor)
            return nullptr;

        return static_cast<HandleType*>(m_descriptor->Get());
    }

    operator bool() const
    {
        if(!m_descriptor)
            false;
        return m_descriptor->Get() != nullptr;
    }
};

template<class To, class From>
Handle<To> HandleCast(Handle<From> other);

class Component
{
    friend class GameObject;
    friend class ComponentManagerBase;

    GameObject* m_gameObject;
    Handle<Component> m_handle;

public:
    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update() {}
    virtual void OnDestroyed() {}

public:
    void SetActive(bool active)
    {
        //m_handle.m_descriptor->manager
    }

    bool IsActive()
    {
        return static_cast<unsigned char>(m_handle.m_descriptor->flags & ComponentFlags::Active);
    }

    Handle<Component> GetHandle()
    {
        return m_handle;
    }

    template<class T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    Handle<T> GetHandle()
    {
        return HandleCast<T>(m_handle);
    }

    template<class T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    Handle<T> GetHandleFast()
    {
        return Handle<T>(m_handle.m_descriptor);
    }
};

class GameObject
{
};


template<>
class is_component<GameObject>
{
public:
    using type = GameObject;

    static constexpr bool value = false;
};



template<class To, class From>
Handle<To> HandleCast(Handle<From> other)
{
    if(!other)
        return Handle<To>();
    if(dynamic_cast<To*>(other.m_descriptor->Get()))
        return Handle<To>(other.m_descriptor);

    return Handle<To>();
}