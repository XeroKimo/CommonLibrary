#include "ApplicationClock.h"

namespace CommonsLibrary
{
    ApplicationClock::ApplicationClock()
    {
        m_initialTick = m_previousTick = m_currentTick = std::chrono::high_resolution_clock::now();
        Tick();
    }
    void ApplicationClock::Tick()
    {
        m_previousTick = m_currentTick;
        m_currentTick = std::chrono::high_resolution_clock::now();
        m_deltaTick = m_previousTick - m_currentTick;
    }
}