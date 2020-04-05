#pragma once
#include <string>

namespace CommonsLibrary
{
    std::string ToLower(const std::string& inputString);

    std::string ToLower(const std::string& inputString, const size_t& startIndex, size_t lengthToCopy);

    namespace Int
    {
        std::string ToString(int value);
        int FromString(std::string value);
    }
    namespace LongLong
    {
        std::string ToString(long long value);
        long long FromString(std::string value);
    }
    namespace Float
    {
        std::string ToString(float value);
        float FromString(std::string value);
    }

    namespace Double
    {
        std::string ToString(double value);
        double FromString(std::string value);
    }
}