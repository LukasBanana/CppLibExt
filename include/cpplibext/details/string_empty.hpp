/*
 * string_empty.hpp file
 * 
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_STRING_EMPTY_H
#define CPPLIBEXT_STRING_EMPTY_H


#include "../cstring_view.hpp"
#include <string>


namespace ext
{

namespace details
{


template
<
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
>
bool string_empty(const std::basic_string<CharT, Traits, Allocator>& s)
{
    return s.empty();
}

template
<
    class CharT,
    class Traits = std::char_traits<CharT>
>
bool string_empty(const basic_cstring_view<CharT, Traits>& s)
{
    return s.empty();
}

template <typename CharT>
bool string_empty(const CharT* s)
{
    return (*s == '\0');
}


} // /namespace details

} // /namespace ext


#endif


