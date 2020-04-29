#pragma once
#include <math.h>
#include <cmath>

namespace CommonsLibrary
{
    constexpr double pi = 3.14159265358979323846264338327950;
    constexpr double rad_To_Deg = 180.0 / pi;
    constexpr double deg_To_Rad = pi / 180.0;

    template <class Type, std::enable_if_t<std::is_floating_point_v<Type>, int> = 0>
    Type ToDegrees(Type radians)
    {
        return static_cast<Type>(static_cast<double>(radians) * rad_To_Deg);
    }
    template <class Type, std::enable_if_t<std::is_floating_point_v<Type>, int> = 0>
    Type ToRadians(Type degrees)
    {
        return static_cast<Type>(static_cast<double>(degrees) * deg_To_Rad);
    }

    constexpr bool use_Left_Handed_Matrices = true;
}