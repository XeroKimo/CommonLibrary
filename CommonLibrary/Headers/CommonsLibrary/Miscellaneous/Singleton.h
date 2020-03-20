#pragma once
#include <type_traits>
#include <vector>
#include <stack>

namespace CommonsLibrary
{
    class SingletonBase
    {
        static std::vector<SingletonBase*> m_singletons;

    protected:
        SingletonBase() = default;

    public:
        static void ShutdownAll()
        {
            while (!m_singletons.empty())
            {
                m_singletons.back()->CallShutdown();
            }
        }

    protected:
        virtual void CallShutdown() = 0;
    protected:
        void RegisterSingleton()
        {
            m_singletons.push_back(this);
        }
        void DeregisterSingleton()
        {
            auto it = std::find(m_singletons.begin(), m_singletons.end(), this);
            m_singletons.erase(it);
        }
    };

    template <class T, class = std::enable_if_t<std::is_class_v<T>>>
    class Singleton : private SingletonBase
    {
        friend class SingletonBase;
    private:
        static T* m_instance;

    protected:
        Singleton() = default;
        ~Singleton() = default;

    public:
        static T* Instance()
        {
            if (!m_instance)
            {
                m_instance = new T();
                m_instance->RegisterSingleton();
            }
            return m_instance;
        }

        void Shutdown()
        {
            if (m_instance)
            {
                m_instance->DeregisterSingleton();
                delete m_instance;
            }
            m_instance = nullptr;
        }

    protected:
        void CallShutdown() override final
        {
            Shutdown();
        }
    };
}