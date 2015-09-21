/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// any_formatter.h
///
/// This file contains any_formatter - a formatter that uses boost::any to erase both the formatter type and object type.
/// This formatter is very similar to type_formatter.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_any_formatter_H
#define BinaryFormatSerializer_any_formatter_H

#include "ISerializer.h"

#include <memory>

#include <boost/any.hpp>

namespace binary_format
{

template<typename TSerializer = ISerializer>
class any_formatter
{
public:
    /// @note The dummy parameter is necessary to be able to actually call this constructor, because
    ///       there is no syntax for crating objects, that accept constructor template parameters.
    ///       Use make_any_formatter() function to create any_formatters more conveniently.
    template<typename T, typename Formatter>
    any_formatter(T* dummy, const Formatter& formatter = Formatter())
        : anyFormatter(new AnyFormatter<T, Formatter>(formatter))
    {
    }

    void save(TSerializer& serializer, const boost::any& object) const
    {
        anyFormatter->save(serializer, object);
    }

    void load(TSerializer& serializer, boost::any& object) const
    {
        anyFormatter->load(serializer, object);
    }

    template<typename T>
    void load(TSerializer& serializer, T& object) const
    {
        boost::any any = T();
        anyFormatter->load(serializer, any);
        object = boost::any_cast<T>(any);
    }

private:
    class IAnyFormatter
    {
    public:
        virtual ~IAnyFormatter() {}
        virtual void save(TSerializer& serializer, const boost::any& object) const = 0;
        virtual void load(TSerializer& serializer, boost::any& object) const = 0;
    };

    template<typename T, typename Formatter>
    class AnyFormatter : public IAnyFormatter
    {
    public:
        AnyFormatter(const Formatter& formatter = Formatter())
            : formatter(formatter)
        {}

    public:
        virtual void save(TSerializer& serializer, const boost::any& object) const
        {
            formatter.save(serializer, boost::any_cast<const T&>(object));
        }

        virtual void load(TSerializer& serializer, boost::any& object) const
        {
            formatter.load(serializer, boost::any_cast<T&>(object));
        }

    private:
        Formatter formatter;
    };

    std::unique_ptr<IAnyFormatter> anyFormatter;
};

template<typename T, typename Formatter>
any_formatter<> make_any_formatter(const Formatter& formatter = Formatter())
{
    return any_formatter<>(static_cast<T*>(nullptr), formatter);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_any_formatter_H
