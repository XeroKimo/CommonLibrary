#pragma once

namespace CommonsLibrary
{

    template <typename ReturnType, typename ...FunctionParameters>
    using FunctionPointer = ReturnType(*)(FunctionParameters...);

    template <class T>
    class Function;

    template <typename ReturnType, typename ...FunctionParameters>
    class Function<ReturnType(FunctionParameters...)> final
    {
        using TFunction = Function<ReturnType(FunctionParameters...)>;
        using TFunctionPtr = FunctionPointer<ReturnType, FunctionParameters...>;
    private:
        TFunctionPtr m_funcPtr = nullptr;

    public:
        Function() = default;

        Function(const TFunction& other) { m_funcPtr = other; }
        void operator=(const TFunction& func) { m_funcPtr = func.m_funcPtr; }
        bool operator==(const TFunction& func) const { return m_funcPtr == func.m_funcPtr; }
        bool operator!=(const TFunction& func) const { return m_funcPtr != func.m_funcPtr; }

        Function(const TFunctionPtr& other) { m_funcPtr = other.m_funcPtr; }
        void operator=(const TFunctionPtr& func) { m_funcPtr = func; }
        bool operator==(const TFunctionPtr& func) const { return m_funcPtr == func; }
        bool operator!=(const TFunctionPtr& func) const { return m_funcPtr != func; }

        ReturnType operator()(FunctionParameters... params) const { return (*m_funcPtr)(params...); }

        bool IsNull() const { return m_funcPtr == nullptr; }
    };
}