/////////////////////////////////////////////////////////////////////////////
/// BinaryFormatSerializer
///    Library for serializing data in arbitrary binary format.
///
/// has_method.h
///
/// This file contains simple member function detector.
/// It should be replaced by Boost Type Traits Introspection Library's has_member_data.hpp (available since Boost 1.54)
/// Or using expression SFINAE when Visual Studio will support it (see http://stackoverflow.com/a/13787502).
///
/// Distributed under Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
/// (c) 2014 Zbigniew Skowron, zbychs@gmail.com
///
/////////////////////////////////////////////////////////////////////////////

#ifndef BinaryFormatSerializer_has_method_H
#define BinaryFormatSerializer_has_method_H

#include "serialization_exceptions.h"

#include <boost/type_traits/integral_constant.hpp>

namespace binary_format
{

#define GENERATE_HAS_METHOD(member, signature) \
    template<typename T> \
    class has_method_impl_ ## member \
    { \
    private: \
        struct WithMember { int member; }; \
        struct MemberChecker : public T, public WithMember {}; \
     \
        typedef char Yes[1]; \
        typedef char No[2]; \
     \
        template<typename U, U> class fail_when_member_ambiguity; \
     \
        template<typename U> \
        static No& check(fail_when_member_ambiguity<int WithMember::*, (signature)&U::member> *); \
        template<typename U> \
        static Yes& check(...); \
     \
    public: \
        enum { value = (sizeof(check<MemberChecker>(0)) == sizeof(Yes)) }; \
    }; \
     \
    template<typename T> \
    class has_method_ ## member : public boost::integral_constant< bool, has_method_impl_ ## member <T>::value > {};

} // namespace binary_format

#endif // BinaryFormatSerializer_has_method_H
