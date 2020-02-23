#pragma once
#include <string>

namespace CommonsLibrary
{
    std::string ToLower(const std::string& inputString)
    {
        std::string outputString;
        for (size_t i = 0; i < inputString.length(); i++)
        {
            outputString += tolower(inputString[i]);
        }
        return outputString;
    }

    std::string ToLower(const std::string& inputString, const size_t& startIndex, size_t lengthToCopy)
    {
        size_t maxIndex = startIndex + lengthToCopy;
        if (maxIndex > inputString.length())
        {
            lengthToCopy = inputString.length() - startIndex;
        }

        return ToLower(inputString.substr(startIndex, lengthToCopy));
    }
}