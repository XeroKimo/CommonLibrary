#pragma once
#include <type_traits>
#include <vector>
#include <stack>

namespace CommonsLibrary
{
    class SingletonBase
    {
        friend class InstantiatedSingletons;
    protected:
        SingletonBase() = default;
    protected:
        virtual void CallShutdown() = 0;
    };

    class InstantiatedSingletons
    {
        template <class T, std::enable_if_t<std::is_class_v<T>, int>>
        friend class Singleton;

        inline static std::vector<SingletonBase*> m_singletons;
    private:
        InstantiatedSingletons();

        static void RegisterSingleton(SingletonBase* singleton)
        {
            m_singletons.push_back(singleton);
        }
        static void DeregisterSingleton(SingletonBase* singleton)
        {
            auto it = std::find(m_singletons.begin(), m_singletons.end(), singleton);
            m_singletons.erase(it);
        }

    public:
        static void ShutdownAll()
        {
            while (!m_singletons.empty())
            {
                m_singletons.back()->CallShutdown();
            }
        }
    };

    template <class T, std::enable_if_t<std::is_class_v<T>, int> = 0>
    class Singleton : private SingletonBase
    {

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
                InstantiatedSingletons::RegisterSingleton(m_instance);
            }
            return m_instance;
        }

        static void Shutdown()
        {
            if (m_instance)
            {
                InstantiatedSingletons::DeregisterSingleton(m_instance);
                delete m_instance;
            }
            m_instance = nullptr;
        }

    private:
        void CallShutdown() override final
        {
            T::Shutdown();
        }
    };

}