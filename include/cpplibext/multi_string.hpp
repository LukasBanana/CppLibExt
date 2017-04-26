/*
 * multi_string.hpp file
 * 
 * Copyright (C) 2014-2017 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_MULTI_STRING_H
#define CPPLIBEXT_MULTI_STRING_H

#if 0//!INCOMPLETE!


#include <string>
#include <vector>


namespace ext
{


/**
Multi string class provides an easy way to access each line of a string container that stores multiple lines of text.
\tparam CharT Specifies the character type. Should be char or wchar_t.
\tparam Traits Specifies the character traits for comparision operators. By default std::char_traits<CharT>.
\tparam Allocator Specifies the memory allocator for the strings and the array list. By default std::allocator<CharT>.
\todo This is incomplete!
*/
template <
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
>
class basic_multi_string
{
    
    public:

        typedef Traits              traits_type;
        typedef Traits::char_type   value_type;

    private:
        
        std::vector<
            std::basic_string<CharT, Traits, Allocator>,
            Allocator
        >
        strings_;

};

typedef basic_multi_string<char> multi_string;
typedef basic_multi_string<wchar_t> multi_wstring;


} // /namespace ext


#endif

#endif


