#pragma once

#include <atomic>
#include <type_traits>

namespace CommonsLibrary
{
    template <class VariableType>
    class ReferencePointerEnableThis;


    template <class VariableType, class = void>
    struct IsReferenceThisEnabled : std::false_type {};

    template <class VariableType>
    struct IsReferenceThisEnabled<VariableType, std::void_t<typename VariableType::RefThis>> :
        std::is_convertible<std::remove_cv_t<VariableType>*, typename VariableType::RefThis*>::type
    {
    };

    class ControlBlock
    {
        std::atomic<unsigned long int> m_counter;
        bool m_exists;

    public:
        ControlBlock() :
            m_counter(1),
            m_exists(true)
        {
        }


    public:

        unsigned long int GetCount() { return m_counter; }
        bool PointerExists() { return m_exists; }
        void DeletePointer() { m_exists = false; }

        static void Equal(ControlBlock** lh, ControlBlock* rh)
        {
            if (*lh != nullptr)
            {
                (*lh)->m_counter--;
                if ((*lh)->m_counter == 0)
                {
                    delete (*lh);
                }
            }
            *lh = rh;
            if (*lh != nullptr)
                (*lh)->m_counter++;
        }
    };


    template <class Type>
    class ReferencePointer
    {
        template <class Derived>
        friend class ReferencePointer;

        template <class Type, class... Params>
        friend ReferencePointer<Type> MakeReference(Params ...variables);

    private:
        Type* m_pointer;
        ControlBlock* m_controlBlock;
        bool m_owner;

    public:
        ReferencePointer() :
            m_pointer(nullptr),
            m_controlBlock(nullptr),
            m_owner(false)
        {
        }
        ReferencePointer(std::nullptr_t) :
            m_pointer(nullptr),
            m_controlBlock(nullptr),
            m_owner(false)
        {
        }
        ReferencePointer(Type* pointer) :
            m_pointer(pointer),
            m_controlBlock(new ControlBlock()),
            m_owner(true)
        {
            if constexpr (std::conjunction_v<IsReferenceThisEnabled<Type>>)
            {
                m_pointer->m_refPointer = *this;
            }
        }
        ReferencePointer(const ReferencePointer& other) :
            m_pointer(other.m_pointer),
            m_controlBlock(nullptr),
            m_owner(false)
        {
            ControlBlock::Equal(&m_controlBlock, other.m_controlBlock);
        }
        template<class DerivedType, class = std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>>>
        ReferencePointer(const ReferencePointer<DerivedType>& other) :
            m_pointer(other.m_pointer),
            m_controlBlock(nullptr),
            m_owner(false)
        {
            ControlBlock::Equal(&m_controlBlock, other.m_controlBlock);
        }
        template<class DerivedType>
        ReferencePointer(const ReferencePointer<DerivedType>& other, Type* pointer) :
            m_pointer(pointer),
            m_controlBlock(nullptr),
            m_owner(false)
        {
            ControlBlock::Equal(&m_controlBlock, other.m_controlBlock);
        }
        ReferencePointer(ReferencePointer&& other) noexcept :
            m_pointer(other.m_pointer),
            m_controlBlock(other.m_controlBlock),
            m_owner(other.m_owner)
        {
            other.m_pointer = nullptr;
            other.m_controlBlock = nullptr;
            other.m_owner = false;
        }
        template<class DerivedType, class = std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>>>
        ReferencePointer(ReferencePointer<DerivedType>&& other) noexcept :
            m_pointer(other.m_pointer),
            m_controlBlock(other.m_controlBlock),
            m_owner(other.m_owner)
        {
            other.m_pointer = nullptr;
            other.m_controlBlock = nullptr;
            other.m_owner = false;
        }

        ~ReferencePointer()
        {
            if (m_owner)
            {
                m_controlBlock->DeletePointer();
                delete m_pointer;
            }
            m_pointer = nullptr;
            ControlBlock::Equal(&m_controlBlock, nullptr);
            m_owner = false;
        }

    public:
        ReferencePointer operator=(std::nullptr_t)
        {
            ResetPointer(nullptr);
            ControlBlock::Equal(&m_controlBlock, nullptr);
            m_owner = false;

            return *this;
        }
        ReferencePointer operator=(const ReferencePointer& other)
        {
            ResetPointer(other.m_pointer);
            ControlBlock::Equal(&m_controlBlock, other.m_controlBlock);
            m_owner = false;

            return *this;
        }
        template<class DerivedType, class = std::enable_if_t<std::is_convertible_v<DerivedType, Type>>>
        ReferencePointer<DerivedType> operator=(const ReferencePointer<DerivedType>& other)
        {
            ResetPointer(other.m_pointer);
            ControlBlock::Equal(&m_controlBlock, other.m_controlBlock);
            m_owner = false;

            return other;
        }
        ReferencePointer& operator=(ReferencePointer&& other) noexcept
        {
            ResetPointer(other.m_pointer);
            m_controlBlock = other.m_controlBlock;
            m_owner = other.m_owner;

            m_pointer = nullptr;
            m_controlBlock = nullptr;
            m_owner = false;

            return *this;
        }
        template<class DerivedType, class = std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>>>
        ReferencePointer& operator=(ReferencePointer<DerivedType>&& other) noexcept
        {
            ResetPointer(other.m_pointer);
            m_controlBlock = other.m_controlBlock;
            m_owner = other.m_owner;

            m_pointer = nullptr;
            m_controlBlock = nullptr;
            m_owner = false;

            return *this;
        }

    public:
        bool operator==(std::nullptr_t) const
        {
            if (m_controlBlock)
                return m_controlBlock->PointerExists() == false;
            return true;
        }
        bool operator!=(std::nullptr_t) const
        {
            if (m_controlBlock)
                return m_controlBlock->PointerExists() == true;
            return false;
        }

        bool operator==(const ReferencePointer& other) const
        {
            return m_controlBlock == other.m_controlBlock;
        }
        bool operator!=(const ReferencePointer& other) const
        {
            return m_controlBlock != other.m_controlBlock;
        }

        template<class DerivedType, class = std::enable_if_t<std::is_convertible_v<DerivedType, Type>>>
        bool operator==(const ReferencePointer<DerivedType>& other) const
        {
            return m_controlBlock == other.m_controlBlock;
        }
        template<class DerivedType, class = std::enable_if_t<std::is_convertible_v<DerivedType, Type>>>
        bool operator!=(const ReferencePointer<DerivedType>& other) const
        {
            return m_controlBlock != other.m_controlBlock;
        }

        operator bool() const
        {
            if (m_controlBlock)
                return m_controlBlock->PointerExists();
            return false;
        }

    public:
        Type* operator->() const { return Get(); }
        Type& operator*() const { return (*Get()); }

    public:
        Type* Get() const { return m_pointer; }

    private:
        void ResetPointer(Type* pointer)
        {
            if (m_owner)
            {
                m_controlBlock->DeletePointer();
                delete m_pointer;
            }
            m_pointer = pointer;
        }

    };

    template <class VariableType>
    class ReferencePointerEnableThis
    {
        template <class Derived>
        friend class ReferencePointer;
    private:
        mutable ReferencePointer<VariableType> m_refPointer;

    public:
        using RefThis = ReferencePointerEnableThis;
        ReferencePointer<VariableType> GetReferencePointer() { return m_refPointer; }
    };

    template <class To, class From>
    [[nodiscard]] ReferencePointer<To> ReferencePointerStaticCast(ReferencePointer<From> other)
    {
        return ReferencePointer<To>(other, static_cast<To*>(other.Get()));
    }

    template <class To, class From>
    [[nodiscard]] ReferencePointer<To> ReferencePointerDynamicCast(ReferencePointer<From> other)
    {
        To* pointer = dynamic_cast<To*>(other.Get());
        if (pointer)
            return ReferencePointer<To>(other, pointer);
        return nullptr;
    }

    template <class Type, class... Params>
    ReferencePointer<Type> MakeReference(Params ...variables)
    {
        return ReferencePointer<Type>(new Type(variables...));
    }
}