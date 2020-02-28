#pragma once
#include <vector>

namespace CommonsLibrary
{
    template <class Type>
    inline bool RemoveFromVector(std::vector<Type>& vector, typename std::vector<Type>::iterator begin, typename std::vector<Type>::iterator end, const Type& objectToRemove)
    {
        auto it = std::find(begin, end, objectToRemove);
        if (it != end)
        {
            vector.erase(it);
            return true;
        }
        return false;
    }
    template <class Type>
    inline bool RemoveFromVector(std::vector<Type>& vector, const Type& objectToRemove)
    {
        return RemoveFromVector(vector, vector.begin(), vector.end(), objectToRemove);
    }
}