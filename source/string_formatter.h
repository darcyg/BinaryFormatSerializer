/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// string_formatter.h
///
/// This file contains string_formatter that formats std::string as length field followed by individual characters.
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_string_formatter_H
#define BinaryFormatSerializer_string_formatter_H

#include <string>

namespace binary_format
{

template<typename SizeFormatter>
class string_formatter
{
    SizeFormatter size_formatter;

public:
    string_formatter(SizeFormatter size_formatter = SizeFormatter())
        : size_formatter(size_formatter)
    {
    }

    void save(ISerializer& serializer, const std::string& string)
    {
        serializer.save(string.length(), size_formatter);
        serializer.saveData(reinterpret_cast<const boost::uint8_t*>(string.c_str()), string.length());
    }

    void load(ISerializer& serializer, std::string& string)
    {
        string.clear();
        size_t string_size;
        serializer.load(string_size, size_formatter);
        if (string_size > 0)
        {
            std::vector<char> data(string_size);
            serializer.loadData(reinterpret_cast<boost::uint8_t*>(&data[0]), string_size);
            string.assign(data.begin(), data.end());
        }
    }
};

template<typename SizeFormatter>
string_formatter<SizeFormatter> create_string_formatter(SizeFormatter size_formatter = SizeFormatter())
{
    return string_formatter<SizeFormatter>(size_formatter);
}

} // namespace binary_format

#endif // BinaryFormatSerializer_string_formatter_H
