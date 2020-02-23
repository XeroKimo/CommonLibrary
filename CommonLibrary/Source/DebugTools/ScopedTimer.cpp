#include "DebugTools/ScopedTimer.h"

namespace CommonsLibrary
{
    ScopedTimer::ScopedTimer() :
        m_initialValue(std::chrono::high_resolution_clock::now())
    {
    }

    ScopedTimer::~ScopedTimer()
    {
    }
}