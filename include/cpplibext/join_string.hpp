/*
 * join_string.hpp file
 * 
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_JOIN_STRING_H
#define CPPLIBEXT_JOIN_STRING_H


#include "cstring_view.hpp"
#include "details/join_sub_string.hpp"

#include <string>
#include <initializer_list>
#include <iterator>


namespace ext
{


/**
\brief Joins the specified string with its values.
\remarks Special characters for the string 's' are: '{', '}', '[', and ']'.

"{0}" will be replaced by the first value from the array 'values',
"{1}" will be replaced by the second value and so froth.
Everything inside squared brackets (e.g. "[optional {0}]") will only be joined to the output string,
if all values inside these brackets are specified and non-empty.

These character will only be treated as direct output,
if the escape character '\\' is written in front of it (e.g. "\\[...\\]"),
to write the escape character itself use "\\\\".

Examples:
\code
join_string("undeclared identifier {0}", { "foo_bar" })            --> "undeclared identifier foo_bar"
join_string("always {0}[, sometimes {1}]", { "first", "second" })  --> "always first, sometimes second"
join_string("always {0}[, sometimes {1}]", { "first", "" })        --> "always first"
join_string("always {0}[, sometimes {1}]", { "first" })            --> "always first"
join_string("one {0}[, two {1}[, three {2}]]", { "1", "2", "3" })  --> "one 1, two 2, three 3"
join_string("one {0}[, two {1}[, three {2}]]", { "1", "", "3" })   --> "one 1"
join_string("one {0}[, two {1}][, three {2}]", { "1", "", "3" })   --> "one 1, three 3"
\endcode

\throws std::out_of_range If a value index, that is not optional, is out of range.
\throws std::invalid_argument If there is an incomplete escape character (e.g. "\\").
\throws std::invalid_argument If there is an incomplete optional part, i.e. a missing closing ']' (e.g. "[").
*/
template
<
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>,
    class InputIterator
>
std::basic_string<CharT, Traits, Allocator> join_string(
    const basic_cstring_view<CharT, Traits>&    s,
    InputIterator                               valuesBegin,
    InputIterator                               valuesEnd)
{
    std::basic_string<CharT, Traits, Allocator> out;
    typename basic_cstring_view<CharT, Traits>::size_type pos = 0;

    /* Verify input iterators */
    if (std::distance(valuesBegin, valuesEnd) < 0)
        throw std::invalid_argument("reversed input iterators in join_string");

    /* Join sub string */
    details::join_sub_string(s, pos, out, valuesBegin, valuesEnd, false);

    /* Check if position has been reached the end of the input string */
    if (pos != s.size())
        throw std::invalid_argument("incomplete optional part in join_string");

    return out;
}

//! Generic wrapper function for ANSI-C strings that shall be joined with values specified by two iterators.
template
<
    class InputIterator,
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
>
std::basic_string<CharT, Traits, Allocator> join_string(
    const CharT*    s,
    InputIterator   valuesBegin,
    InputIterator   valuesEnd)
{
    return join_string<CharT, Traits, Allocator>(std::basic_string<CharT>(s), valuesBegin, valuesEnd);
}

//! Wrapper function for ANSI-C strings.
template
<
    class ValueT,
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
>
std::basic_string<CharT, Traits, Allocator> join_string(
    const CharT*                            s,
    const std::initializer_list<ValueT>&    values)
{
    return join_string<CharT, Traits, Allocator>(basic_cstring_view<CharT>(s), std::begin(values), std::end(values));
}

//! Wrapper function for std::basic_string.
template
<
    class ValueT,
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
>
std::basic_string<CharT, Traits, Allocator> join_string(
    const basic_cstring_view<CharT, Traits>&    s,
    const std::initializer_list<ValueT>&        values)
{
    return join_string<CharT, Traits, Allocator>(s, std::begin(values), std::end(values));
}

//! Wrapper function for ext::basic_cstring_view.
template
<
    class ValueT,
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
>
std::basic_string<CharT, Traits, Allocator> join_string(
    const std::basic_string<CharT, Traits, Allocator>&  s,
    const std::initializer_list<ValueT>&                values)
{
    return join_string<CharT, Traits, Allocator>(basic_cstring_view<CharT>(s), std::begin(values), std::end(values));
}


} // /namespace ext


#endif


