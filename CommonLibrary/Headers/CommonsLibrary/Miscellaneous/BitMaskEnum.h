#pragma once
#include <type_traits>

template <int size>
struct __ReturnType
{
    using type = int;
};
template <>
struct __ReturnType<1>
{
    using type = char;
};
template <>
struct __ReturnType<2>
{
    using type = short;
};
template <>
struct __ReturnType<4>
{
    using type = int;
};
template <>
struct __ReturnType<8>
{
    using type = long long;
};

template <int Size>
using ReturnType = typename __ReturnType<Size>::type;

template <class Type, int Size = sizeof(Type), class Enable = std::enable_if_t<std::is_enum_v<Type>>>
Type operator|(const Type& lh, const Type& rh)
{
    return static_cast<Type>(static_cast<ReturnType<Size>>(lh) | static_cast<ReturnType<Size>>(rh));
}

template <class Type, int Size = sizeof(Type), class Enable = std::enable_if_t<std::is_enum_v<Type>>>
Type operator|=(const Type& lh, const Type& rh)
{
    return static_cast<Type>(static_cast<ReturnType<Size>>(lh) | static_cast<ReturnType<Size>>(rh));
}

template <class Type, int Size = sizeof(Type), class Enable = std::enable_if_t<std::is_enum_v<Type>>>
Type operator&(const Type& lh, const Type& rh)
{
    return static_cast<Type>(static_cast<ReturnType<Size>>(lh) & static_cast<ReturnType<Size>>(rh));
}

template <class Type, int Size = sizeof(Type), class Enable = std::enable_if_t<std::is_enum_v<Type>>>
Type operator&=(const Type& lh, const Type& rh)
{
    return static_cast<Type>(static_cast<ReturnType<Size>>(lh) & static_cast<ReturnType<Size>>(rh));
}

template <class Type, int Size = sizeof(Type), class Enable = std::enable_if_t<std::is_enum_v<Type>>>
Type operator^(const Type& lh, const Type& rh)
{
    return static_cast<Type>(static_cast<ReturnType<Size>>(lh) ^ static_cast<ReturnType<Size>>(rh));
}

template <class Type, int Size = sizeof(Type), class Enable = std::enable_if_t<std::is_enum_v<Type>>>
Type operator~(const Type& lh)
{
    return static_cast<Type>(~static_cast<ReturnType<Size>>(lh));
}
