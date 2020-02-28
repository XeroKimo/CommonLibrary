#pragma once
#include <string>

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
}