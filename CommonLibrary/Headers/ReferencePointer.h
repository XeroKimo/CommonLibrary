#pragma once
#pragma once
#include <atomic>
#include <stdexcept>
#include <type_traits>

namespace CommonsLibrary
{
    class NullPointerException : public std::runtime_error
    {
    public:
        NullPointerException() : runtime_error("Pointer is nullptr") {}
    };

    class __ReferenceBlock
    {
    private:
        std::atomic<unsigned long int> m_counter;
        bool m_exists;
    public:
        __ReferenceBlock()
        {
            m_counter = 1;
            m_exists = true;
        }

        void Increment() { m_counter++; }
        void Decrement() { m_counter--; }
        unsigned long int GetCount() { return m_counter; }
        bool PointerExists() { return m_exists; }
        void DeletePointer() { m_exists = false; }
    };

    template <class VariableType>
    class __ReferencePointerBase
    {
        template <class DerivedType>
        friend class __ReferencePointerBase;

    protected:
        VariableType* m_pointer = nullptr;
        __ReferenceBlock* m_reference_block = nullptr;

    public:
        __ReferencePointerBase() = default;
        __ReferencePointerBase(VariableType* pointer)
        {
            DecrementReferenceBlock();
            m_pointer = pointer;
            m_reference_block = new __ReferenceBlock();
        }

    protected:
        void ConstructFromOther(const __ReferencePointerBase& other)
        {
            DecrementReferenceBlock();
            if (other.m_reference_block == nullptr)
                return;

            m_pointer = other.m_pointer;
            m_reference_block = other.m_reference_block;
            m_reference_block->Increment();
        }

        template <class DerivedType>
        void ConstructFromDerived(const __ReferencePointerBase<DerivedType>& other)
        {
            DecrementReferenceBlock();
            if (other.m_reference_block == nullptr)
                return;

            m_pointer = other.m_pointer;
            m_reference_block = other.m_reference_block;
            m_reference_block->Increment();
        }

        template <class DerivedType>
        void ConstructFromCast(const __ReferencePointerBase<DerivedType>& other, VariableType* pointer)
        {
            DecrementReferenceBlock();
            if (other.m_reference_block == nullptr)
                return;
            m_pointer = pointer;
            m_reference_block = other.m_reference_block;
            m_reference_block->Increment();
        }

        void ConstructFromMove(__ReferencePointerBase& other)
        {
            DecrementReferenceBlock();
            m_pointer = other.m_pointer;
            m_reference_block = other.m_reference_block;

            other.m_pointer = nullptr;
            other.m_reference_block = nullptr;
        }

    protected:
        void DeletePointer()
        {
            DeletePointerInternal();
            DecrementReferenceBlock();
        }

        void DecrementReferenceBlock()
        {
            if (!m_reference_block)
                return;

            m_reference_block->Decrement();
            if (m_reference_block->GetCount() == 0)
            {
                DeletePointerInternal();

                delete m_reference_block;
            }
            m_reference_block = nullptr;
        }

    protected:
        void DeletePointerInternal()
        {
            if (!m_reference_block)
                return;

            if (!m_reference_block->PointerExists())
                return;

            m_reference_block->DeletePointer();

            delete m_pointer;
            m_pointer = nullptr;
        }
    };

    template <class VariableType>
    class ReferenceFromThis;


    template <class VariableType, class = void>
    struct __ReferenceEnabledFromThis : std::false_type {};

    template <class VariableType>
    struct __ReferenceEnabledFromThis<VariableType, std::void_t<typename VariableType::RefThis>> : 
        std::is_convertible<std::remove_cv_t<VariableType>*, typename VariableType::RefThis*>::type {};

    template <class VariableType>
    class ReferencePointer : private __ReferencePointerBase<VariableType>
    {
        template <class DerivedType>
        friend class ReferencePointer;

        template <class VariableType, class... types>
        friend ReferencePointer<VariableType> MakeReference(types&& ...variables);
        using BaseClass = __ReferencePointerBase<VariableType>;

    private:
        bool m_owner = false;
    public:
        constexpr ReferencePointer() noexcept = default;
        constexpr ReferencePointer(std::nullptr_t) noexcept {};

        ReferencePointer(VariableType* pointer)
        {
            TakeOwnership(pointer, new __ReferenceBlock());
        }
        ReferencePointer(const ReferencePointer& other)
        {
            if (m_owner)
                BaseClass::DeletePointerInternal();
            BaseClass::ConstructFromOther(other);
            m_owner = false;
        }
        ReferencePointer(ReferencePointer&& other) noexcept
        {
            if (m_owner)
                BaseClass::DeletePointerInternal();
            BaseClass::ConstructFromMove(other);
            m_owner = other.m_owner;
            other.m_owner = false;
        }

        template<class DerivedType, class = std::enable_if_t<std::is_convertible_v<DerivedType, VariableType>>>
        ReferencePointer(const ReferencePointer<DerivedType>& other)
        {
            if (m_owner)
                BaseClass::DeletePointerInternal();
            BaseClass::ConstructFromDerived(other);
            m_owner = false;
        }
        template<class DerivedType>
        ReferencePointer(const ReferencePointer<DerivedType>& other, VariableType* pointer)
        {
            if (m_owner)
                BaseClass::DeletePointerInternal();
            BaseClass::ConstructFromCast(other, pointer);
            m_owner = false;
        }
        template<class DerivedType>
        ReferencePointer(ReferencePointer<DerivedType>&& other) noexcept
        {
            if (m_owner)
                BaseClass::DeletePointerInternal();
            BaseClass::DecrementReferenceBlock();
            BaseClass::m_pointer = other.m_pointer;
            BaseClass::m_reference_block = other.m_reference_block;

            other.m_pointer = nullptr;
            other.m_reference_block = nullptr;

            m_owner = other.m_owner;
            other.m_owner = false;
        }


        ~ReferencePointer()
        {
            if (m_owner)
                BaseClass::DeletePointer();
            else
                BaseClass::DecrementReferenceBlock();
        }

    public:
        VariableType* operator->() const
        {
#if _DEBUG
            try
            {
                if (BaseClass::m_reference_block == nullptr)
                    throw NullPointerException();
                if (!BaseClass::m_reference_block->PointerExists())
                    throw NullPointerException();
                return Get();
            }
            catch (NullPointerException e)
            {
                throw;
            }
#else
            return Get();
#endif
        }

        VariableType& operator*() const {
#if _DEBUG
            try
            {
                if (BaseClass::m_reference_block == nullptr)
                    throw NullPointerException();
                if (!BaseClass::m_reference_block->PointerExists())
                    throw NullPointerException();
                return *Get();
            }
            catch (NullPointerException e)
            {
                throw;
            }
#else
            return *Get();
#endif
        }
        void operator=(std::nullptr_t)
        {
            if (m_owner)
                BaseClass::DeletePointer();
            else
            {
                BaseClass::DecrementReferenceBlock();
                BaseClass::m_pointer = nullptr;
            }
        }
        void operator=(const ReferencePointer& other)
        {
            BaseClass::ConstructFromOther(other);
            m_owner = false;
        }

        template<class DerivedType, class = std::enable_if_t<std::is_convertible_v<DerivedType, VariableType>>>
        void operator=(const ReferencePointer<DerivedType>& other)
        {
            BaseClass::ConstructFromDerived(other);
        }
        void operator=(ReferencePointer&& other)
        {
            BaseClass::ConstructFromMove(other);
            m_owner = other.m_owner;
            other.m_owner = false;
        }

        bool operator==(std::nullptr_t) const
        {
            return (BaseClass::m_reference_block) ? BaseClass::m_reference_block->PointerExists() : true;
        }
        bool operator!=(std::nullptr_t) const
        {
            return (BaseClass::m_reference_block) ? BaseClass::m_reference_block->PointerExists() : false;
        }

        bool operator==(const ReferencePointer& other) const
        {
            return (BaseClass::m_reference_block && other.m_reference_block) ? BaseClass::m_reference_block == other.m_reference_block : false;
        }
        bool operator!=(const ReferencePointer& other) const
        {
            return (BaseClass::m_reference_block && other.m_reference_block) ? BaseClass::m_reference_block != other.m_reference_block : true;
        }

        template<class DerivedType, class = std::enable_if_t<std::is_convertible_v<DerivedType, VariableType>>>
        bool operator==(const ReferencePointer<DerivedType>& other) const
        {
            return (BaseClass::m_reference_block && other.m_reference_block) ? BaseClass::m_reference_block == other.m_reference_block : false;
        }
        template<class DerivedType, class = std::enable_if_t<std::is_convertible_v<DerivedType, VariableType>>>
        bool operator!=(const ReferencePointer<DerivedType>& other) const
        {
            return (BaseClass::m_reference_block && other.m_reference_block) ? BaseClass::m_reference_block != other.m_reference_block : true;
        }

        operator bool() const
        {
            return *this != nullptr;
        }

    public:

        VariableType* Get() const { return BaseClass::m_pointer; }

    private:
        template <class Derived>
        void TakeOwnership(Derived* const pointer, __ReferenceBlock* const block)
        {
            if (m_owner)
                BaseClass::DeletePointerInternal();
            BaseClass::m_pointer = pointer;
            BaseClass::m_reference_block = block;
            if constexpr (std::conjunction_v<__ReferenceEnabledFromThis<Derived>>)
            {
                pointer->m_refPointer = ReferencePointer<VariableType>(*this);
            }
            m_owner = true;
        }
    };

    template <class VariableType>
    class ReferenceFromThis
    {
        template <class Derived>
        friend class ReferencePointer;
    private:
        mutable ReferencePointer<VariableType> m_refPointer;

    public:
        using RefThis = ReferenceFromThis;
        ReferencePointer<VariableType> GetReferencePointer() { return m_refPointer; }
    };

    template <class DerivedType, class base_type>
    [[nodiscard]] ReferencePointer<DerivedType> ReferencePointerStaticCast(ReferencePointer<base_type> other)
    {
        DerivedType* pointer = static_cast<DerivedType*>(other.Get());
        return ReferencePointer<DerivedType>(other, pointer);
    }

    template <class DerivedType, class base_type>
    [[nodiscard]] ReferencePointer<DerivedType> ReferencePointerDynamicCast(ReferencePointer<base_type> other)
    {
        DerivedType* pointer = dynamic_cast<DerivedType*>(other.Get());
        return ReferencePointer<DerivedType>(other, pointer);
    }

    template <class VariableType, class... types>
    ReferencePointer<VariableType> MakeReference(types&& ...variables)
    {
        VariableType* ptr = new VariableType(std::forward<types>(variables)...);
        __ReferenceBlock* block = new __ReferenceBlock();
        ReferencePointer<VariableType> ref;
        ref.TakeOwnership(ptr, block);
        return ref;
    }
}