/*
 * string_append.hpp file
 * 
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_STRING_APPEND_H
#define CPPLIBEXT_STRING_APPEND_H


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
void string_append(std::basic_string<CharT, Traits, Allocator>& out, const std::basic_string<CharT, Traits, Allocator>& appendix)
{
    out.append(appendix);
}

template
<
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
>
void string_append(std::basic_string<CharT, Traits, Allocator>& out, const basic_cstring_view<CharT, Traits>& appendix)
{
    out.append(appendix.c_str());
}

template
<
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
>
void string_append(std::basic_string<CharT, Traits, Allocator>& out, const CharT* appendix)
{
    out.append(appendix);
}


} // /namespace details

} // /namespace ext


#endif


