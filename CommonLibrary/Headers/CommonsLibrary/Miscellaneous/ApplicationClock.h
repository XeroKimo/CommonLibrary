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

        template<class ChronoType = std::chrono::nanoseconds, class Enbale = std::enable_if_t<std::_Is_specialization_v<ChronoType, std::chrono::duration>>>
        ChronoType GetChronoDeltaTime() { return std::chrono::duration_cast<ChronoType>(m_deltaTick); }
        template<class ChronoType = std::chrono::nanoseconds, class Enbale = std::enable_if_t<std::_Is_specialization_v<ChronoType, std::chrono::duration>>>
        ChronoType GetChronoLifeTime() { return std::chrono::duration_cast<ChronoType>(std::chrono::high_resolution_clock::now() - m_initialTick); }

        template <typename Type, class Enable = std::enable_if_t<std::is_fundamental_v<Type>>>
        Type GetDeltaTime() { return std::chrono::duration<Type>(m_deltaTick).count(); }
        template <typename Type, class Enable = std::enable_if_t<std::is_fundamental_v<Type>>>
        Type GetLifeTime() { return std::chrono::duration<Type>(std::chrono::high_resolution_clock::now() - m_initialTick).count(); }
    };
}