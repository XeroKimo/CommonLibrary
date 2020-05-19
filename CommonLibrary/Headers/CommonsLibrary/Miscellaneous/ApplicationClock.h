#pragma once
#include <chrono>

namespace CommonsLibrary
{
    class ApplicationClock
    {
        using time_point = std::chrono::high_resolution_clock::time_point;
    private:
        time_point m_initialTick;
        time_point m_previousTick;
        time_point m_currentTick;
        std::chrono::nanoseconds m_deltaTick;

    public:
        ApplicationClock();

    public:
        void Tick();       

        template<class Duration = std::chrono::nanoseconds, std::enable_if_t<std::_Is_specialization_v<Duration, std::chrono::duration>, int> = 0>
        Duration GetChronoDeltaTime() { return std::chrono::duration_cast<Duration>(m_deltaTick); }
        template<class Duration = std::chrono::nanoseconds, std::enable_if_t<std::_Is_specialization_v<Duration, std::chrono::duration>, int> = 0>
        Duration GetChronoLifeTime() { return std::chrono::duration_cast<Duration>(std::chrono::high_resolution_clock::now() - m_initialTick); }

        template <typename Type, std::enable_if_t<std::is_fundamental_v<Type>, int> = 0>
        Type GetDeltaTime() { return std::chrono::duration<Type>(m_deltaTick).count(); }
        template <typename Type, std::enable_if_t<std::is_fundamental_v<Type>, int> = 0>
        Type GetLifeTime() { return std::chrono::duration<Type>(std::chrono::high_resolution_clock::now() - m_initialTick).count(); }
    };
}