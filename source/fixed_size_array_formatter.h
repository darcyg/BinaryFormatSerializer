/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// fixed_size_array_formatter.h
///
/// This file contains fixed_size_array_formatter that formats an array as a sequence of elements. Array length is not serialized.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_fixed_size_array_formatter_H
#define BinaryFormatSerializer_fixed_size_array_formatter_H

#include "unified_formatter_base.h"
#include "verbatim_formatter.h"

#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_array.hpp>

namespace binary_format
{

namespace detail
{
} // namespace detail

template<typename ValueFormatter, int ArraySize = -1>
class fixed_size_array_formatter : public unified_formatter_base< fixed_size_array_formatter<ValueFormatter, ArraySize> >
{
    ValueFormatter value_formatter;

public:
    fixed_size_array_formatter(ValueFormatter value_formatter = ValueFormatter())
        : value_formatter(value_formatter)
    {
    }

#if 0
    template<int Size, typename ValueType>
    void serialize(ISerializer& serializer, ValueType(&array)[Size]) const
    {
        BOOST_STATIC_ASSERT(ArraySize <= Size);
        const int array_size = (ArraySize == -1) ? Size : ArraySize;
        for (int i = 0; i < array_size; ++i)
        {
            serializer.serialize(array[i], value_formatter);
        }
    }
#endif

    template<typename ValueType, typename TSerializer>
    void serialize(TSerializer& serializer, ValueType* array) const
    {
        BOOST_STATIC_ASSERT(ArraySize >= 0);
        for (int i = 0; i < ArraySize; ++i)
        {
            serializer.serialize(array[i], value_formatter);
        }
    }
};

template<typename ValueFormatter>
class fixed_size_array_formatter<ValueFormatter, -1> : public unified_formatter_base< fixed_size_array_formatter<ValueFormatter, -1> >
{
    ValueFormatter value_formatter;

public:
    fixed_size_array_formatter(ValueFormatter value_formatter = ValueFormatter())
        : value_formatter(value_formatter)
    {
    }

    template<int Size, typename ValueType, typename TSerializer>
    void serialize(TSerializer& serializer, ValueType(&array)[Size]) const
    {
        for (int i = 0; i < Size; ++i)
        {
            serializer.serialize(array[i], value_formatter);
        }
    }
};


template<typename ValueFormatter, int ArraySize = -1>
fixed_size_array_formatter<ValueFormatter, ArraySize> create_fixed_size_array_formatter(ValueFormatter value_formatter = ValueFormatter())
{
    return fixed_size_array_formatter<ValueFormatter, ArraySize>(value_formatter);
}


/// @brief fixed_size_array_formatter<ValueFormatter, ArraySize> is a verbatim formatter if ValueFormatter is.
template<typename ValueFormatter, int ArraySize, typename T>
struct is_verbatim_formatter< fixed_size_array_formatter<ValueFormatter, ArraySize>, T > : public is_verbatim_formatter<ValueFormatter, T>
{};

} // namespace binary_format

#endif // BinaryFormatSerializer_fixed_size_array_formatter_H
