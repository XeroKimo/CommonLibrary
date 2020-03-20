#pragma once
#include <assert.h>

#ifdef NDEBUG

#define assertError(expression) ((void)0)

#else

#define assertError(expression, message)   (void)((!!(expression)) || (_wassert(_CRT_WIDE(#message), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)) 

#endif
