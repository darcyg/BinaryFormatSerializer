/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// endian_formatter.h
///
/// This file contains little_endian and big_endian formatters for POD types.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_endian_formatter_H
#define BinaryFormatSerializer_endian_formatter_H

#include "uint_of_size.h"
#include "endian_shuffle.h"
#include "serialization_exceptions.h"

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/exception/error_info.hpp>

namespace binary_format
{

namespace detail
{

template<int Size, typename T>
struct cant_store_type_in_this_number_of_bytes
{
    typedef boost::error_info<struct tag_cant_store_type_in_this_number_of_bytes, T> errinfo;
};

template<typename endian_tag, int Size>
class endian_formatter
{
public:
    /// @brief Stores given pod in endian_tag byte order.
    ///        Throws lossy_conversion if Size is not enough to represent actual run-time value of pod.
    template<typename T>
    void save(ISerializer& serializer, const T& pod)
    {
        BOOST_STATIC_ASSERT(boost::is_pod<T>::value);

        const uint_of_size<sizeof(T)>::type* value = reinterpret_cast<const uint_of_size<sizeof(T)>::type*>(&pod);
        uint_of_size<Size>::type resized_value = *value;

        // throw if conversion to sized_value was lossy
        if (resized_value != *value)
        {
            BOOST_THROW_EXCEPTION((lossy_conversion() << detail::cant_store_type_in_this_number_of_bytes<Size, T>::errinfo(pod)));
        }

        uint_of_size<Size>::type endian_shuffled_value = native_to<endian_tag>(resized_value);
        serializer.serializeData(reinterpret_cast<boost::uint8_t*>(&endian_shuffled_value), Size);
    }

    /// @brief Loads given pod from endian_tag byte order.
    template<typename T>
    void load(ISerializer& serializer, T& pod)
    {
        BOOST_STATIC_ASSERT(boost::is_pod<T>::value);

        uint_of_size<Size>::type endian_shuffled_value;
        serializer.serializeData(reinterpret_cast<boost::uint8_t*>(&endian_shuffled_value), Size);

        uint_of_size<Size>::type resized_value = native_to<endian_tag>(endian_shuffled_value);
        uint_of_size<sizeof(T)>::type* value = reinterpret_cast<uint_of_size<sizeof(T)>::type*>(&pod);
        *value = resized_value;
    }
};

} // namespace detail

template<int Size>
class little_endian : public detail::endian_formatter<little_endian_tag, Size>
{
};

template<int Size>
class big_endian : public detail::endian_formatter<big_endian_tag, Size>
{
};

} // namespace binary_format

#endif // BinaryFormatSerializer_endian_formatter_H