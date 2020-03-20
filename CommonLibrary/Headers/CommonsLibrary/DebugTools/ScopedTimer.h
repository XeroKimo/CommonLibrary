#pragma once
#include <chrono>
#include "CommonsLibrary/Delegates/Delegate.h"

namespace CommonsLibrary
{
    template <class ChronoType = std::chrono::nanoseconds, class FundamentalType = double, class EnableChrono = std::enable_if_t<std::_Is_specialization_v<ChronoType, std::chrono::duration>>, class EnableFundamental = std::enable_if_t<std::is_fundamental_v<FundamentalType>>>
    class ScopedTimer
    {
    public:
        using OnDestroy = Delegate<void(ChronoType, FundamentalType)>;
    private:
        OnDestroy m_onDestroy;
        std::chrono::high_resolution_clock::time_point m_initialValue;
    public:
        ScopedTimer() :
            m_onDestroy(nullptr), 
            m_initialValue(std::chrono::high_resolution_clock::now()) { }

        ScopedTimer(OnDestroy onDestroy) :
            m_onDestroy(onDestroy),
            m_initialValue(std::chrono::high_resolution_clock::now()) { }
        ~ScopedTimer()
        {
            ChronoType chronoLifeTime = GetChronoLifeTime();
            FundamentalType fundamentalLifeTime = std::chrono::duration<FundamentalType>(chronoLifeTime).count();
            if (!m_onDestroy.IsNull())
                m_onDestroy(chronoLifeTime, fundamentalLifeTime);
        };

        ChronoType GetChronoLifeTime() const { return std::chrono::duration_cast<ChronoType>(std::chrono::high_resolution_clock::now() - m_initialValue); }

        FundamentalType GetLifeTime() const { return std::chrono::duration<FundamentalType>(std::chrono::high_resolution_clock::now() - m_initialValue).count(); }
    };
}