#pragma once
#include <chrono>

namespace CommonsLibrary
{
    class ScopedTimer
    {
    private:
        std::chrono::high_resolution_clock::time_point m_initialValue;

    public:
        ScopedTimer();
        ~ScopedTimer();

        template <class ChronoType, class Enable = std::enable_if_t<std::_Is_specialization_v<ChronoType, std::chrono::duration>>>
        ChronoType GetChronoLifeTime() const { return std::chrono::duration_cast<ChronoType>(std::chrono::high_resolution_clock::now() - m_initialValue); }

        template <class FundamentalType, class Enable = std::enable_if_t<std::is_fundamental_v<FundamentalType>>>
        FundamentalType GetLifeTime() const { return std::chrono::duration<FundamentalType>(std::chrono::high_resolution_clock::now() - m_initialValue).count(); }
    };
}