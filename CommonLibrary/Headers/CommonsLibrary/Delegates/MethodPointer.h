#pragma once
#include "FunctionPointer.h"

namespace CommonsLibrary
{
    template <class T, typename ReturnType, typename ...FunctionParameters>
    using MethodPointer = ReturnType(T::*)(FunctionParameters...);

    template <class T, class Func>
    class Method;

    template <class T, typename ReturnType, typename ...FunctionParameters>
    class Method<T, ReturnType(FunctionParameters...)> final
    {
        using TMethod = Method<T, ReturnType(FunctionParameters...)>;
        using TMethodPtr = MethodPointer<T, ReturnType, FunctionParameters...>;
    private:
        TMethodPtr m_funcPtr = nullptr;

    public:
        Method() = default;

        Method(const TMethod& other)
        {
            m_funcPtr = other;
        }
        void operator=(const TMethod& func)
        {
            m_funcPtr = func.m_funcPtr;
        }
        bool operator==(const TMethod& func) const
        {
            return m_funcPtr == func.m_funcPtr;
        }
        bool operator!=(const TMethod& func) const
        {
            return m_funcPtr != func.m_funcPtr;
        }

        Method(TMethodPtr other)
        {
            m_funcPtr = other.m_funcPtr;
        }
        void operator=(const TMethodPtr& func)
        {
            m_funcPtr = func;
        }
        bool operator==(const TMethodPtr& func) const
        {
            return m_funcPtr == func;
        }
        bool operator!=(const TMethodPtr& func) const
        {
            return m_funcPtr != func;
        }

        ReturnType operator()(T* const object, FunctionParameters... params) const
        {
            return (object->*m_funcPtr)(params...);
        }

        bool IsNull() { return m_funcPtr == nullptr; }

    };
}