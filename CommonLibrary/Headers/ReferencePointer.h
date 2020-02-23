#pragma once
#include <atomic>


namespace CommonsLibrary
{
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
            m_pointer = other.m_pointer;
            m_reference_block = other.m_reference_block;

            other.m_pointer = nullptr;
            other.m_reference_block = nullptr;
        }

    protected:
        void DeletePointerInternal()
        {
            if (!m_reference_block)
                return;

            if (!m_reference_block->PointerExists())
                return;

            m_reference_block->DeletePointer();
            DecrementReferenceBlock();

            delete m_pointer;
            m_pointer = nullptr;
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
    };

    template <class VariableType>
    class ReferencePointer : private __ReferencePointerBase<VariableType>
    {
        template <class DerivedType>
        friend class ReferencePointer;

        using BaseClass = __ReferencePointerBase<VariableType>;

    private:
        bool m_owner = false;
    public:
        constexpr ReferencePointer() noexcept = default;
        constexpr ReferencePointer(std::nullptr_t) noexcept {};

        ReferencePointer(VariableType* pointer) : BaseClass(pointer)
        {
            m_owner = true;
        }
        ReferencePointer(const ReferencePointer& other)
        {
            BaseClass::ConstructFromOther(other);
            m_owner = false;
        }
        ReferencePointer(ReferencePointer&& other)
        {
            BaseClass::ConstructFromMove(other);
            m_owner = other.m_owner;
            other.m_owner = false;
        }

        template<class DerivedType>
        ReferencePointer(const ReferencePointer<DerivedType>& other, VariableType* pointer)
        {
            BaseClass::ConstructFromCast(other, pointer);
            m_owner = false;
        }

        template<class DerivedType, class = std::enable_if_t<std::_SP_pointer_compatible<DerivedType, VariableType>::value>>
        ReferencePointer(const ReferencePointer<DerivedType>& other)
        {
            BaseClass::ConstructFromDerived(other);
            m_owner = false;
        }

        ~ReferencePointer()
        {
            if (m_owner)
                BaseClass::DeletePointerInternal();
            else
                BaseClass::DecrementReferenceBlock();
        }

    public:
        VariableType* operator->() { return get(); }
        void operator=(std::nullptr_t)
        {
            if (m_owner)
                BaseClass::DeletePointerInternal();
            else
            {
                BaseClass::DecrementReferenceBlock();
                BaseClass::m_pointer = nullptr;
            }
        }
        void operator=(const ReferencePointer& other)
        {
            BaseClass::ConstructFromOther(other);
        }

        void operator=(ReferencePointer&& other)
        {
            BaseClass::ConstructFromMove(other);
            m_owner = other.m_owner;
            other.m_owner = false;
        }
        template<class DerivedType, class = std::enable_if_t<std::_SP_pointer_compatible<DerivedType, VariableType>::value>>
        void operator=(const ReferencePointer<DerivedType>& other)
        {
            BaseClass::ConstructFromDerived(other);
        }

        bool operator==(std::nullptr_t)
        {
            return (BaseClass::m_reference_block) ? BaseClass::m_reference_block->PointerExists() : true;
        }
        bool operator!=(std::nullptr_t)
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

        template<class DerivedType, class = std::enable_if_t<std::_SP_pointer_compatible<DerivedType, VariableType>::value>>
        bool operator==(const ReferencePointer<DerivedType>& other) const
        {
            return (BaseClass::m_reference_block && other.m_reference_block) ? BaseClass::m_reference_block == other.m_reference_block : false;
        }
        template<class DerivedType, class = std::enable_if_t<std::_SP_pointer_compatible<DerivedType, VariableType>::value>>
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
    };

    template <class DerivedType, class base_type>
    [[nodiscard]] ReferencePointer<DerivedType> ReferencePointerStaticCast(ReferencePointer<base_type> other)
    {
        DerivedType* pointer = static_cast<DerivedType*>(other.get());
        return ReferencePointer<DerivedType>(other, pointer);
    }

    template <class DerivedType, class base_type>
    [[nodiscard]] ReferencePointer<DerivedType> ReferencePointerDynamicCast(ReferencePointer<base_type> other)
    {
        DerivedType* pointer = dynamic_cast<DerivedType*>(other.get());
        return ReferencePointer<DerivedType>(other, pointer);
    }

    template <class VariableType, class... types>
    ReferencePointer<VariableType> MakeReference(types&& ...variables)
    {
        return ReferencePointer<VariableType>(new VariableType(std::forward<types>(variables)...));
    }
}