/*
 * cstring_len.hpp file
 *
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_CSTRING_LEN_H
#define CPPLIBEXT_CSTRING_LEN_H


#include <cstring>
#include <cwchar>


namespace ext
{


//! Returns the length of the specified string.
template <class CharT>
std::size_t cstring_len(const CharT* s)
{
    std::size_t len = 0;
    while (*(s++) != 0)
        ++len;
    return len;
}

//! Template specialization that uses std::strlen.
template <>
std::size_t cstring_len<char>(const char* s)
{
    return std::strlen(s);
}

//! Template specialization that uses std::wcslen.
template <>
std::size_t cstring_len(const wchar_t* s)
{
    return std::wcslen(s);
}


} // /namespace ext


#endif


