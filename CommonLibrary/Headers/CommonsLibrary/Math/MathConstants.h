#pragma once
#include <math.h>
#include <cmath>

namespace CommonsLibrary
{
    constexpr double PI = 3.14159265358979323846264338327950;
    constexpr double RAD_TO_DEG = 180.0 / PI;
    constexpr double DEG_TO_RAD = PI / 180.0;

    template <class Type, std::enable_if_t<std::is_fundamental_v<Type>, int> = 0>
    Type ToDegrees(Type radians)
    {
        return static_cast<Type>(static_cast<double>(radians) * RAD_TO_DEG);
    }
    template <class Type, std::enable_if_t<std::is_fundamental_v<Type>, int> = 0>
    Type ToRadians(Type degrees)
    {
        return static_cast<Type>(static_cast<double>(degrees) * DEG_TO_RAD);
    }

    constexpr bool USE_LEFT_HANDED_MATRICES = false;
}