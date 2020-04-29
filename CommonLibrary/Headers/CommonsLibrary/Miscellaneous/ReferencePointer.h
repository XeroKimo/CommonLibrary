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
            if(*lh != nullptr)
            {
                (*lh)->m_counter--;
                if((*lh)->m_counter == 0)
                {
                    delete (*lh);
                }
            }
            *lh = rh;
            if(*lh != nullptr)
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

        friend struct std::hash<ReferencePointer>;
    private:
        Type* m_pointer = nullptr;
        ControlBlock* m_controlBlock = nullptr;
        bool m_owner = false;

    public:
        ReferencePointer() {}
        ReferencePointer(std::nullptr_t) {}
        ReferencePointer(Type* pointer) :
            m_pointer(pointer),
            m_controlBlock(new ControlBlock()),
            m_owner(true)
        {
            if constexpr(std::conjunction_v<IsReferenceThisEnabled<Type>>)
            {
                m_pointer->m_refPointer = *this;
            }
        }
        ReferencePointer(const ReferencePointer& other) :
            m_pointer(other.m_pointer)
        {
            ControlBlock::Equal(&m_controlBlock, other.m_controlBlock);
        }

        template<class DerivedType, std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>, int> = 0>
        ReferencePointer(const ReferencePointer<DerivedType>& other) :
            m_pointer(other.m_pointer)
        {
            ControlBlock::Equal(&m_controlBlock, other.m_controlBlock);
        }
        template<class DerivedType>
        ReferencePointer(const ReferencePointer<DerivedType>& other, Type* pointer) :
            m_pointer(pointer)
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
        template<class DerivedType, std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>, int> = 0>
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
            if(m_owner)
            {
                m_controlBlock->DeletePointer();
                delete m_pointer;
            }
            ControlBlock::Equal(&m_controlBlock, nullptr);
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
        template<class DerivedType, std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>, int> = 0>
        ReferencePointer<DerivedType> operator=(const ReferencePointer<DerivedType>& other)
        {
            ResetPointer(other.m_pointer);
            ControlBlock::Equal(&m_controlBlock, other.m_controlBlock);
            m_owner = false;

            return other;
        }
        ReferencePointer& operator=(ReferencePointer&& other) noexcept
        {
            if(other == *this && m_owner == other.m_owner)
                return *this;

            ResetPointer(other.m_pointer);
            m_controlBlock = other.m_controlBlock;
            m_owner = other.m_owner;

            other.m_pointer = nullptr;
            other.m_controlBlock = nullptr;
            other.m_owner = false;

            return *this;
        }
        template<class DerivedType, std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>, int> = 0>
        ReferencePointer& operator=(ReferencePointer<DerivedType>&& other) noexcept
        {
            if(other == *this && m_owner == other.m_owner)
                return *this;

            ResetPointer(other.m_pointer);
            m_controlBlock = other.m_controlBlock;
            m_owner = other.m_owner;

            other.m_pointer = nullptr;
            other.m_controlBlock = nullptr;
            other.m_owner = false;

            return *this;
        }

    public:
        //bool operator==(std::nullptr_t) const
        //{
        //    if(m_controlBlock)
        //        return m_controlBlock->PointerExists() == false;
        //    return true;
        //}
        //bool operator!=(std::nullptr_t) const
        //{
        //    if(m_controlBlock)
        //        return m_controlBlock->PointerExists() == true;
        //    return false;
        //}

        bool operator==(Type* other) const
        {
            if(m_controlBlock)
            {
                if(m_controlBlock->PointerExists())
                    return m_pointer == other;
            }
            return other == nullptr;
        }

        bool operator!=(Type* other) const
        {
            if(m_controlBlock)
            {
                if(m_controlBlock->PointerExists())
                    return m_pointer != other;
            }
            return other != nullptr;
        }

        bool operator==(const ReferencePointer& other) const
        {
            if(m_controlBlock && other.m_controlBlock)
            {
                if(m_controlBlock->PointerExists() && other.m_controlBlock->PointerExists())
                    return m_controlBlock == other.m_controlBlock;
            }
            return false;
        }
        bool operator!=(const ReferencePointer& other) const
        {
            if(m_controlBlock && other.m_controlBlock)
            {
                if(m_controlBlock->PointerExists() && other.m_controlBlock->PointerExists())
                    return m_controlBlock != other.m_controlBlock;
            }
            return true;
        }

        template<class DerivedType, std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>, int> = 0>
        bool operator==(const ReferencePointer<DerivedType>& other) const
        {
            if(m_controlBlock && other.m_controlBlock)
            {
                if(m_controlBlock->PointerExists() && other.m_controlBlock->PointerExists())
                    return m_controlBlock == other.m_controlBlock;
            }
            return false;
        }
        template<class DerivedType, std::enable_if_t<std::is_convertible_v<DerivedType*, Type*>, int> = 0>
        bool operator!=(const ReferencePointer<DerivedType>& other) const
        {
            if(m_controlBlock && other.m_controlBlock)
            {
                if(m_controlBlock->PointerExists() && other.m_controlBlock->PointerExists())
                    return m_controlBlock != other.m_controlBlock;
            }
            return true;
        }

        operator bool() const
        {
            if(m_controlBlock)
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
            if(m_owner)
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
        if(pointer)
            return ReferencePointer<To>(other, pointer);
        return nullptr;
    }

    template <class Type, class... Params>
    ReferencePointer<Type> MakeReference(Params ...variables)
    {
        return ReferencePointer<Type>(new Type(variables...));
    }

    template<class T>
    struct std::hash<ReferencePointer<T>>
    {
        std::size_t operator()(const CommonsLibrary::ReferencePointer<T>& s) const noexcept
        {
            std::size_t h1 = std::hash<T*>{}(s.m_pointer);
            std::size_t h2 = std::hash<bool>{}(s.m_controlBlock->PointerExists());
            return h1 ^ (h2 << 1); // or use boost::hash_combine
        }
    };

}