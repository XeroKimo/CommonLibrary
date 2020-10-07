#pragma once

namespace CommonsLibrary
{
    template<class Type>
    struct OptOutParam
    {

        explicit OptOutParam() = default;
        explicit OptOutParam(std::nullptr_t) {}
        explicit OptOutParam(Type& pointer) : data(&pointer) {}

        void operator=(const Type& object)
        {
            if(data)
                *data = object;
        }

    private:
        Type* const data = nullptr;
    };

    template<class Type>
    struct OutParam
    {
        explicit OutParam(Type& reference) : data(reference) {}

        void operator=(const Type& object)
        {
            data = object;
        }

    private:
        Type& data;
    };
}