#pragma once
#include <vector>
#include "FunctionPointer.h"
#include "MethodPointer.h"

namespace CommonsLibrary
{
    template <class T>
    class Delegate;

    template <typename ReturnType, typename ...FunctionParameters>
    class Delegate<ReturnType(FunctionParameters...)> final
    {
        using TFunctionWrapper = Function<ReturnType(void*, FunctionParameters...)>;
        using TDelegate = Delegate<ReturnType(FunctionParameters...)>;
        using TFunctionPtr = FunctionPointer<ReturnType, FunctionParameters...>;

        template <class T>
        using TMethodPtr = MethodPointer<T, ReturnType, FunctionParameters...>;

    private:
        void* m_boundObject = nullptr;
        TFunctionWrapper m_functionWrapper;
    public:

        Delegate() = default;
        Delegate(const TDelegate& other)
        {
            m_boundObject = other.m_boundObject;
            m_functionWrapper = other.m_functionWrapper;
        }

        TDelegate& operator=(const TDelegate& other)
        {
            m_boundObject = other.m_boundObject;
            m_functionWrapper = other.m_functionWrapper;
            return *this;
        }
        bool operator==(const TDelegate& other) const
        {
            return m_boundObject == other.m_boundObject && m_functionWrapper == other.m_functionWrapper;
        }
        bool operator!=(const TDelegate& other) const
        {
            return m_boundObject != other.m_boundObject && m_functionWrapper != other.m_functionWrapper;
        }
        ReturnType operator()(FunctionParameters... params) const
        {
            return m_functionWrapper(m_boundObject, params...);
        }

        bool IsNull() const
        {
            return m_functionWrapper.IsNull();
        }
        void Unbind()
        {
            m_boundObject = nullptr;
            m_functionWrapper = nullptr;
        }
        void Swap(TDelegate& other)
        {
            TDelegate temp = other;
            other = *this;
            *this = temp;
        }



        template<TFunctionPtr func>
        void Bind()
        {
            m_boundObject = nullptr;
            m_functionWrapper = FunctionWrapper<func>;
        }
        template<class T, TMethodPtr<T> function>
        void Bind(T* object)
        {
            m_boundObject = object;
            m_functionWrapper = MethodWrapper<T, function>;
        }

        template <TFunctionPtr func>
        static Delegate Generate()
        {
            TDelegate delegate;
            delegate.Bind<func>();
            return delegate;
        }
        template <class T, TMethodPtr<T> func>
        static TDelegate Generate(T* object)
        {
            TDelegate delegate;
            delegate.Bind<T, func>(object);
            return delegate;
        }

    private:
        template <TFunctionPtr Function>
        static ReturnType FunctionWrapper(void* object, FunctionParameters... params)
        {
            return (*Function)(params...);
        }
        template <class T, TMethodPtr<T> Function>
        static ReturnType MethodWrapper(void* object, FunctionParameters... params)
        {
            T* ptr = static_cast<T*>(object);
            return (ptr->*Function)(params...);
        }
    };
}
#define BindDelegateFunction(functionSignature, function) Delegate<functionSignature>::Generate<function>();
#define BindDelegateMethod(Class, functionSignature, function, object) Delegate<functionSignature>::Generate<Class, Class:functionSignature>(Class* object);
