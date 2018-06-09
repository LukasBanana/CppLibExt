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

#include <string>
#include <stdexcept>
#include <initializer_list>
#include <iterator>


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
bool is_string_empty(const std::basic_string<CharT, Traits, Allocator>& s)
{
    return s.empty();
}

template <typename CharT>
bool is_string_empty(const CharT* s)
{
    return (*s == '\0');
}

/*
Returns true, if all values have been joined to the output string,
i.e. they are set for the repsective index {N} and they are non-empty.
*/
template
<
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>,
    class InputIterator
>
bool join_sub_string
    (
        const basic_cstring_view<CharT, Traits>&                in,
        typename basic_cstring_view<CharT, Traits>::size_type&  pos,
        std::basic_string<CharT, Traits, Allocator>&            out,
        InputIterator                                           valuesBegin,
        InputIterator                                           valuesEnd,
        bool                                                    optional
    )
{
    using string_type   = std::basic_string<CharT, Traits, Allocator>;
    using size_type     = typename string_type::size_type;

    bool escapeChar         = false;
    bool replacedAllValues  = true;

    const auto numValues = static_cast<size_type>(std::distance(valuesBegin, valuesEnd));

    if (numValues < 0)
        throw std::invalid_argument("reversed range iterators in join_string");

    for (auto num = in.size(); pos < num;)
    {
        /* Get next character */
        auto c = in[pos++];

        if (escapeChar)
        {
            /* Add character without transformation to output string */
            out.push_back(c);
            escapeChar = false;
        }
        else
        {
            if (c == CharT('\\'))
            {
                /* Next character will be added without transformation */
                escapeChar = true;
            }
            else if (c == CharT('{'))
            {
                /* Parse index N in '{N}' */
                string_type idxStr;
                while (pos < num)
                {
                    /* Get next character */
                    c = in[pos++];
                    if (c == CharT('}'))
                        break;
                    else
                        idxStr.push_back(c);
                }

                /* Get value by index from array */
                const auto idx = static_cast<size_type>(std::stoul(idxStr));
                if (idx < numValues)
                {
                    /* Append value to output string */
                    const auto& val = *(valuesBegin + idx);
                    if (is_string_empty(val))
                        replacedAllValues = false;
                    else
                        out.append(val);
                }
                else if (optional)
                {
                    /* This sub string will not be added to the final output string */
                    replacedAllValues = false;
                }
                else
                {
                    /* If this value replacement was not optional -> error */
                    throw std::out_of_range(
                        "index (" + std::to_string(idx) + ") out of range [0, " +
                        std::to_string(numValues) + ") in join_string"
                    );
                }
            }
            else if (c == CharT('['))
            {
                /* Parse optional part with recursive call */
                string_type outOpt;
                if (join_sub_string(in, pos, outOpt, valuesBegin, valuesEnd, true))
                    out.append(outOpt);
            }
            else if (c == CharT(']'))
            {
                /* Close optional part and return from recursive call */
                break;
            }
            else
            {
                /* Add current character to output string */
                out.push_back(c);
            }
        }
    }

    if (escapeChar)
        throw std::invalid_argument("incomplete escape character in report string");

    return replacedAllValues;
}


} // /namespace details


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


