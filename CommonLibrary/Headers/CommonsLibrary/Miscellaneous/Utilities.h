#pragma once

namespace CommonsLibrary
{
    template <class Type>
    struct OptionalOutParam
    {
        Type* data;

        OptionalOutParam(Type& pointer) : data(&pointer) {}
        OptionalOutParam(std::nullptr_t) : data(nullptr) {}

        void operator=(Type* pointer)
        {
            if (data)
                data = pointer;
        }
        void operator=(const Type& object)
        {
            if (data)
                *data = object;
        }
    };

    template <class Type>
    struct RequiredOutParam
    {
        Type& data;
        RequiredOutParam(Type& output) : data(output) {}

        void operator=(const Type& object)
        {
            data = object;
        }
    };

}