#pragma once
#include <chrono>
#include "../Delegates/Delegate.h"

namespace CommonsLibrary
{
    class ScopedTimer
    {
    public:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;

        using Output = Delegate<void(Clock::duration)>;
    private:
        Output m_delegate;
        TimePoint m_initialPoint = Clock::now();
    public:
        ScopedTimer() = default;
        ScopedTimer(Output callback) : m_delegate(callback)
        {

        }
        
        ~ScopedTimer()
        {
            if(!m_delegate.IsNull())
            {
                m_delegate(Clock::now() - m_initialPoint);
            }
        }

        template<class Duration = std::chrono::nanoseconds, std::enable_if_t<std::_Is_specialization_v<Duration, std::chrono::duration>, int> = 0>
        typename Duration::rep GetLifeTime()
        {
            return std::chrono::duration_cast<Duration>(Clock::now() - m_initialPoint).count();
        }
        
        template<class Type = long double, std::enable_if_t<std::is_fundamental_v<Type>, int> = 0>
        Type GetLifeTimeAs()
        {
            return std::chrono::duration<Type>(Clock::now() - m_initialPoint).count();
        }

    public:
        static void ConsoleOutput(Clock::duration duration);
    };
}