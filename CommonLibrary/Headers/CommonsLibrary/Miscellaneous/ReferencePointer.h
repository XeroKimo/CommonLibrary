#pragma once
#pragma warning(push)
#pragma warning(disable : 4150)
#include <atomic>
#include <type_traits>
#include <memory>

namespace CommonsLibrary
{
    template <class VariableType>
    class ReferenceViewEnableThis;

    template <class VariableType, class = void>
    struct IsReferenceThisEnabled : std::false_type {};

    template <class VariableType>
    struct IsReferenceThisEnabled<VariableType, std::void_t<typename VariableType::RefThis>> :
        std::is_convertible<std::remove_cv_t<VariableType>*, typename VariableType::RefThis*>::type
    {
    };

    template <class Type>
    class ReferencePointer;

    template <class Type, std::enable_if_t<std::negation_v<std::is_pointer<Type>>, int> = 0>
    class ReferenceObject;

    template <class Type>
    class ReferenceView;

    template <class To, class From>
    [[nodiscard]] ReferenceView<To> ReferencePointerStaticCast(ReferenceView<From> other);

    template <class To, class From>
    [[nodiscard]] ReferenceView<To> ReferencePointerDynamicCast(ReferenceView<From> other);





    template <class Type>
    class ReferencePointer
    {
        template <class Derived>
        friend class ReferencePointer;

        template <class Derived>
        friend class ReferenceView;

        template <class Type, class... Params>
        friend ReferencePointer<Type> MakeReference(Params ...variables);

        friend struct std::hash<ReferencePointer>;
    private:
        std::shared_ptr<bool> m_exists;
        std::unique_ptr<Type> m_pointer;

    public:
        ReferencePointer() : m_exists(std::make_shared<bool>(false)) {}
        ReferencePointer(std::nullptr_t) : m_exists(std::make_shared<bool>(false)) {}
        ReferencePointer(Type* pointer) :
            m_exists(std::make_shared<bool>(true)),
            m_pointer(std::unique_ptr<Type>(pointer))
        {
            if constexpr(std::conjunction_v<IsReferenceThisEnabled<Type>>)
            {
                m_pointer->m_refPointer = *this;
            }
        }
        ReferencePointer(std::unique_ptr<Type> pointer) :
            m_exists(std::make_shared<bool>(true)),
            m_pointer(std::move(pointer))
        {
            if constexpr(std::conjunction_v<IsReferenceThisEnabled<Type>>)
            {
                m_pointer->m_refPointer = *this;
            }
        }

        ReferencePointer(const ReferencePointer& other) = delete;
        template<class DerivedType, std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>, int> = 0>
        ReferencePointer(const ReferencePointer<DerivedType>& other) = delete;

        ReferencePointer(ReferencePointer&& other) noexcept :
            m_exists(std::move(other.m_exists)),
            m_pointer(std::move(other.m_pointer))
        {
        }
        template<class DerivedType, std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>, int> = 0>
        ReferencePointer(ReferencePointer<DerivedType>&& other) noexcept :
            m_exists(std::move(other.m_exists)),
            m_pointer(std::move(other.m_pointer))
        {
        }

        ~ReferencePointer()
        {
            (*m_exists) = false;
        }

    public:
        void operator=(std::nullptr_t)
        {
            m_pointer = nullptr;
            (*m_exists) = false;
        }

        ReferencePointer operator=(const ReferencePointer& other) = delete;
        template<class DerivedType, std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>, int> = 0>
        ReferencePointer<DerivedType> operator=(const ReferencePointer<DerivedType>& other) = delete;

        void operator=(ReferencePointer&& other) noexcept
        {
            m_exists = std::move(other.m_exists);
            m_pointer = std::move(other.m_pointer);
        }
        template<class DerivedType, std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>, int> = 0>
        void operator=(ReferencePointer<DerivedType>&& other) noexcept
        {
            m_exists = std::move(other.m_exists);
            m_pointer = std::move(other.m_pointer);
        }

    public:
        bool operator==(std::nullptr_t)
        {
            return *m_exists == false;
        }
        bool operator!=(std::nullptr_t)
        {
            return *m_exists != false;
        }

        operator bool() const
        {
            return *m_exists;
        }

    public:
        Type* operator->() const { return Get(); }
        Type& operator*() const { return (*Get()); }

    public:
        Type* Get() const { return m_pointer.get(); }

        bool IsAlive() const { return *m_exists; }
        void Swap(ReferencePointer& other)
        {
            ReferencePointer temp = std::move(other);
            other = std::move(*this);
            (*this) = std::move(temp);
        }
    };


    template<class Type, std::enable_if_t<std::negation_v<std::is_pointer<Type>>, int>>
    class ReferenceObject
    {
        template<class Derived>
        friend class ReferenceView;

    private:
        std::shared_ptr<bool> m_exists = std::make_shared<bool>(true);

    public:
        Type object;

    public:
        ReferenceObject()
        {
            if constexpr(std::conjunction_v<IsReferenceThisEnabled<Type>>)
            {
                object.m_refPointer = *this;
            }
        }
        ReferenceObject(const Type defaultValue) :
            object(defaultValue)
        {
            if constexpr(std::conjunction_v<IsReferenceThisEnabled<Type>>)
            {
                object->m_refPointer = *this;
            }
        }
        ReferenceObject(const Type& defaultValue) :
            object(defaultValue)
        {
            if constexpr(std::conjunction_v<IsReferenceThisEnabled<Type>>)
            {
                object->m_refPointer = *this;
            }
        }
        ReferenceObject(Type&& defaultValue) noexcept :
            object(std::move(defaultValue))
        {
            if constexpr(std::conjunction_v<IsReferenceThisEnabled<Type>>)
            {
                object->m_refPointer = *this;
            }
        }

        template<class... Args>
        ReferenceObject(const Args ...args) :
            object(args...)
        {
            if constexpr(std::conjunction_v<IsReferenceThisEnabled<Type>>)
            {
                object->m_refPointer = *this;
            }
        }

        ReferenceObject(const ReferenceObject& other) = delete;
        ReferenceObject(ReferenceObject&& other) noexcept = default;

        ReferenceObject operator=(const ReferenceObject& other) = delete;
        ReferenceObject& operator=(ReferenceObject&& other) noexcept = default;

        ~ReferenceObject()
        {
            (*m_exists) = false;
        }


        bool IsAlive() const { return *m_exists; }
    };





    template <class Type>
    class ReferenceView
    {
        template<class Derived>
        friend class ReferenceView;

        template <class Derived>
        friend class ReferenceViewEnableThis;

        template <class To, class From>
        friend ReferenceView<To> ReferenceViewStaticCast(ReferenceView<From> other);

        template <class To, class From>
        friend ReferenceView<To> ReferenceViewDynamicCast(ReferenceView<From> other);

    private:
        std::shared_ptr<bool> m_exists = std::make_shared<bool>(false);
        Type* m_pointer = nullptr;


    public:
        ReferenceView() = default;
        ReferenceView(std::nullptr_t)
        {
        }


        ReferenceView(ReferenceObject<Type>& object) :
            m_exists(object.m_exists),
            m_pointer(&object.object)
        {
        }
        template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, Type*>, int> = 0>
        ReferenceView(ReferenceObject<Derived>& other) :
            m_exists(other.m_exists),
            m_pointer(&other.object)
        {

        }

        ReferenceView(const ReferencePointer<Type>& pointer) :
            m_exists(pointer.m_exists),
            m_pointer(pointer.m_pointer.get())
        {

        }
        template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, Type*>, int> = 0>
        ReferenceView(const ReferencePointer<Derived>& other) :
            m_exists(other.m_exists),
            m_pointer(other.m_pointer.get())
        {

        }

        ReferenceView(Type* pointer, std::shared_ptr<bool>& exists) :
            m_exists(exists),
            m_pointer(pointer)
        {

        }

        ReferenceView(const ReferenceView& other) :
            m_exists(other.m_exists),
            m_pointer(other.m_pointer)
        {
        }
        ReferenceView(ReferenceView&& other) noexcept :
            m_exists(std::move(other.m_exists)),
            m_pointer(std::move(other.m_pointer))
        {
        }
        template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, Type*>, int> = 0>
        ReferenceView(const ReferenceView<Derived>& other) :
            m_exists(other.m_exists),
            m_pointer(other.m_pointer)
        {

        }
        template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, Type*>, int> = 0>
        ReferenceView(ReferenceView<Derived>&& other) :
            m_exists(std::move(other.m_exists)),
            m_pointer(std::move(other.m_pointer))
        {

        }

        ReferenceView operator=(ReferenceObject<Type>& other)
        {
            m_exists = other.m_exists;
            m_pointer = &other.object;
            return *this;
        }
        template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, Type*>, int> = 0>
        ReferenceView operator=(const ReferenceObject<Derived>& other)
        {
            m_exists = other.m_exists;
            m_pointer = &other.m_object;
            return *this;
        }

        ReferenceView operator=(const  ReferencePointer<Type>& other)
        {
            m_exists = other.m_exists;
            m_pointer = other.m_pointer.get();
            return *this;
        }
        template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, Type*>, int> = 0>
        ReferenceView operator=(ReferencePointer<Derived>& other)
        {
            m_exists = other.m_exists;
            m_pointer = other.m_pointer.get();
            return *this;
        }

        ReferenceView operator=(const ReferenceView& other)
        {
            m_exists = other.m_exists;
            m_pointer = other.m_pointer;
            return *this;
        }
        template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, Type*>, int> = 0>
        ReferenceView operator=(ReferenceView<Derived>& other)
        {
            m_exists = other.m_exists;
            m_pointer = other.m_pointer;
            return *this;
        }

        ReferenceView& operator=(ReferenceView&& other) noexcept = default;
        template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, Type*>, int> = 0>
        ReferenceView& operator=(ReferenceView<Derived>&& other)
        {
            m_exists = std::move(other.m_exists);
            m_pointer = std::move(other.m_pointer);
            return *this;
        }

        bool operator==(Type* other) const
        {
            if(*m_exists)
                return other == m_pointer;
            return other == nullptr;
        }
        bool operator!=(Type* other) const
        {
            if(*m_exists)
                return other != m_pointer;
            return other != nullptr;
        }
        bool operator==(ReferenceView& other) const
        {
            if((*m_exists) && (*other.m_exists))
                return other.m_pointer == m_pointer;
            return false;
        }
        bool operator!=(ReferenceView& other) const
        {
            if((*m_exists) && (*other.m_exists))
                return other.m_pointer != m_pointer;
            return true;
        }
        template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, Type*>, int> = 0>
        bool operator==(ReferenceView<Derived>& other) const
        {
            if((*m_exists) && (*other.m_exists))
                return other.m_pointer == m_pointer;
            return false;
        }
        template<class Derived, std::enable_if_t<std::is_convertible_v<Derived*, Type*>, int> = 0>
        bool operator!=(ReferenceView<Derived>& other) const
        {
            if((*m_exists) && (*other.m_exists))
                return other.m_pointer != m_pointer;
            return true;
        }

        operator bool() const
        {
            return *m_exists;
        }

        Type& operator*() const
        {
            return *m_pointer;
        }

        Type* operator->() const
        {
            return m_pointer;
        }

    public:
        Type* Get() const
        {
            return m_pointer;
        }
        bool IsAlive() const { return *m_exists; }

        template<class Derived>
        ReferenceView<Derived> StaticCast() { return ReferenceViewStaticCast(*this); }
        template<class Derived>
        ReferenceView<Derived> DynamicCast() { return ReferenceViewDynamicCast(*this); }
    };




    template <class Type>
    class ReferenceViewEnableThis
    {
        template <class Derived>
        friend class ReferencePointer;

        template<class Derived, std::enable_if_t<std::negation_v<std::is_pointer<Type>>, int>>
        friend class ReferenceObject;
    private:
        mutable ReferenceView<Type> m_refPointer;

    public:
        using RefThis = ReferenceViewEnableThis;
        ReferenceView<Type> GetReferencePointer() const { return m_refPointer; }
    };




    template <class To, class From>
    [[nodiscard]] ReferenceView<To> ReferenceViewStaticCast(ReferenceView<From> other)
    {
        return ReferenceView<To>(static_cast<To*>(other.Get()), other.m_exists);
    }

    template <class To, class From>
    [[nodiscard]] ReferenceView<To> ReferenceViewDynamicCast(ReferenceView<From> other)
    {
        To* pointer = dynamic_cast<To*>(other.Get());
        if(pointer)
            return ReferenceView<To>(pointer, other.m_exists);
        return nullptr;
    }

    template <class Type, class... Params>
    ReferencePointer<Type> MakeReference(Params ...variables)
    {
        return ReferencePointer<Type>(new Type(variables...));
    }

    template<class Type>
    bool operator==(const ReferenceView<Type>& lh, const ReferencePointer<Type>& rh)
    {
        if(lh.IsAlive() && rh.IsAlive())
        {
            return lh.Get() == rh.Get();
        }
        return false;
    }
    template<class Type>
    bool operator==(const ReferencePointer<Type>& lh, const ReferenceView<Type>& rh)
    {
        if(lh.IsAlive() && rh.IsAlive())
        {
            return lh.Get() == rh.Get();
        }
        return false;
    }
    template<class Type>
    bool operator!=(const ReferenceView<Type>& lh, const ReferencePointer<Type>& rh)
    {
        if(lh.IsAlive() && rh.IsAlive())
        {
            return lh.Get() != rh.Get();
        }
        return true;
    }
    template<class Type>
    bool operator!=(const ReferencePointer<Type>& lh, const ReferenceView<Type>& rh)
    {
        if(lh.IsAlive() && rh.IsAlive())
        {
            return lh.Get() != rh.Get();
        }
        return true;
    }


    template<class Type>
    bool operator==(const ReferenceView<Type>& lh, const ReferenceObject<Type>& rh)
    {
        if(lh.IsAlive() && rh.IsAlive())
        {
            return lh.Get() == &rh.object;
        }
        return false;
    }
    template<class Type>
    bool operator==(const ReferenceObject<Type>& lh, const ReferenceView<Type>& rh)
    {
        if(lh.IsAlive() && rh.IsAlive())
        {
            return rh.Get() == &lh.object;
        }
        return false;
    }
    template<class Type>
    bool operator!=(const ReferenceView<Type>& lh, const ReferenceObject<Type>& rh)
    {
        if(lh.IsAlive() && rh.IsAlive())
        {
            return lh.Get() != &rh.object;
        }
        return true;
    }
    template<class Type>
    bool operator!=(const ReferenceObject<Type>& lh, const ReferenceView<Type>& rh)
    {
        if(lh.IsAlive() && rh.IsAlive())
        {
            return rh.Get() != &lh.object;
        }
        return true;
    }

    template<class Type>
    struct std::hash<ReferencePointer<Type>>
    {
        std::size_t operator()(const CommonsLibrary::ReferencePointer<Type>& s) const noexcept
        {
            std::size_t h1 = std::hash<Type*>{}(s.m_pointer);
            std::size_t h2 = std::hash<bool>{}(s.m_controlBlock->PointerExists());
            return h1 ^ (h2 << 1); // or use boost::hash_combine
        }
    };

}
#pragma warning(pop)