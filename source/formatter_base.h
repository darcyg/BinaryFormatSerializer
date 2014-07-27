/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// formatter_base.h
///
/// This file contains formatter_base mixin class that provides save() and load() by the means of the serialize() method.
/// NOTE: I think this class might misbehave when used for formatters that have child formatters. And maybe in other circumstances too.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_formatter_base_H
#define BinaryFormatSerializer_formatter_base_H

#include "ISerializer.h"

namespace binary_format
{

template<typename Derived, typename TSerializer = ISerializer>
class formatter_base
{
public:
    template<typename T>
    void save(TSerializer& serializer, const T& object)
    {
        static_cast<Derived*>(this)->serialize(serializer, const_cast<T&>(object));
    }

    template<typename T>
    void load(TSerializer& serializer, T& object)
    {
        static_cast<Derived*>(this)->serialize(serializer, object);
    }
};

} // namespace binary_format

#endif // BinaryFormatSerializer_formatter_base_H
