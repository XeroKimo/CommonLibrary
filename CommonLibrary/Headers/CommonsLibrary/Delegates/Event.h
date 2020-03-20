#pragma once
#include "Delegate.h"

namespace CommonsLibrary
{
    template <class T>
    class Event;

    template <typename ReturnType, typename ...FunctionParameters>
    class Event<ReturnType(FunctionParameters...)>
    {
        using TDelegate = Delegate<ReturnType(FunctionParameters...)>;
        using TFunctionPtr = FunctionPointer<ReturnType, FunctionParameters...>;

        template <class T>
        using TMethodPtr = MethodPointer<T, ReturnType, FunctionParameters...>;
    private:
        std::vector<TDelegate> m_delegates;

    public:
        Event() = default;
        //Event(const Event& other) = delete;

        //Event& operator=(const Event& other) = delete;
        void operator+=(const TDelegate& other) { Bind(other); }
        void operator-=(const TDelegate& other) { Unbind(other); }
        void operator()(FunctionParameters... params) const { FireEvent(params...); }
        void operator()(std::vector<ReturnType>& returnValues, FunctionParameters... params) const { FireEvent(returnValues, params...); }

        void Bind(const TDelegate& other)
        {
            if (other.IsNull())
                return;
            m_delegates.push_back(other);
        }
        void Unbind(const TDelegate& other)
        {
            if (other.IsNull())
                return;
            int i = 0;
            for (const auto& delegate : m_delegates)
            {
                if (other == delegate)
                {
                    m_delegates[i].Swap(m_delegates[m_delegates.size() - 1]);
                    m_delegates.pop_back();
                    return;
                }
                i++;
            }
        }

        size_t Size() const { return m_delegates.size(); }
        bool Empty() const { return m_delegates.empty(); }

        template<TFunctionPtr function>
        void Bind()
        {
            m_delegates.push_back(TDelegate());
            m_delegates.back().Bind<function>();
        }
        template<TFunctionPtr function>
        void Unbind()
        {
            TDelegate func;
            func.Bind<function>();
            int i = 0;
            for (auto& delegate : m_delegates)
            {
                if (func == delegate)
                {
                    m_delegates[i].swap(m_delegates[m_delegates.size() - 1]);
                    m_delegates.pop_back();
                    return;
                }
                i++;
            }
        }

        template<class T, TMethodPtr<T> function>
        void Bind(T* object)
        {
            if (!object)
                return;
            m_delegates.push_back(TDelegate());
            m_delegates.back().Bind<T, function>(object);
        }
        template<class T, TMethodPtr<T> function>
        void Unbind(T* object)
        {
            if (object == nullptr)
                return;
            TDelegate func;
            func.Bind<T, function>(object);
            int i = 0;
            for (auto& delegate : m_delegates)
            {
                if (func == delegate)
                {
                    auto it = m_delegates.begin();
                    m_delegates[i].swap(m_delegates[m_delegates.size() - 1]);
                    m_delegates.pop_back();
                    return;
                }
                i++;
            }
        }

    private:
        void FireEvent(FunctionParameters... params) const
        {
            for (auto& delegate : m_delegates)
            {
                delegate(params...);
            }
        }
        void FireEvent(std::vector<ReturnType>& returnValues, FunctionParameters... params) const
        {
            returnValues.reserve(m_delegates.size());
            for (auto& delegate : m_delegates)
            {
                returnValues.push_back(delegate(params...));
            }
        }
    };
}