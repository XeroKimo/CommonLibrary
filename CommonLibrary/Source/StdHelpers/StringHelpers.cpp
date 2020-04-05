#include "CommonsLibrary/StdHelpers/StringHelpers.h"

namespace CommonsLibrary
{
    std::string ToLower(const std::string& inputString)
    {
        std::string outputString;
        outputString.resize(inputString.length());
        for (size_t i = 0; i < inputString.length(); i++)
        {
            outputString[i] = tolower(inputString[i]);
        }
        return outputString;
    }

    std::string ToLower(const std::string& inputString, const size_t& startIndex, size_t lengthToCopy)
    {
        size_t maxIndex = startIndex + lengthToCopy;
        size_t maxLength = inputString.length();
        if (maxIndex > maxLength)
        {
            lengthToCopy = maxLength - startIndex;
        }

        return ToLower(inputString.substr(startIndex, lengthToCopy));
    }

    namespace Int
    {
        std::string ToString(int value)
        {
            return std::to_string(value);
        }

        int FromString(std::string value)
        {
            return std::stoi(value);
        }
    }
    namespace LongLong
    {
        std::string ToString(long long value)
        {
            return std::to_string(value);
        }

        long long FromString(std::string value)
        {
            return std::stoll(value);
        }
    }
    namespace Float
    {
        std::string ToString(float value)
        {
            return std::to_string(value);
        }

        float FromString(std::string value)
        {
            return std::stof(value);
        }
    }
    namespace Double
    {
        std::string ToString(double value)
        {
            return std::to_string(value);
        }

        double FromString(std::string value)
        {
            return std::stod(value);
        }
    }
}