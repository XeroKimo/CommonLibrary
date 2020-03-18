#pragma once
#include <unordered_map>

namespace CommonsLibrary
{
    template<class Key, class Value>
    bool KeyExists(const std::unordered_map<Key, Value>& map, const Key& keyToFind)
    {
        return map.count(keyToFind) == 1;
    }
}