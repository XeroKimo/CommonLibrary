#include "CommonsLibrary/DebugTools/ScopedTimer.h"
#include <iostream>

void CommonsLibrary::ScopedTimer::ConsoleOutput(Clock::duration duration)
{
    std::cout << duration.count() << "ns\n";
}
