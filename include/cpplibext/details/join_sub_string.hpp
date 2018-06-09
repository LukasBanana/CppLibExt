/*
 * join_sub_string.hpp file
 * 
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_JOIN_SUB_STRING_H
#define CPPLIBEXT_JOIN_SUB_STRING_H


#include "../cstring_view.hpp"
#include "string_empty.hpp"
#include "string_append.hpp"

#include <string>
#include <stdexcept>


namespace ext
{

namespace details
{


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
                    if (string_empty(val))
                        replacedAllValues = false;
                    else
                        string_append(out, val);
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

} // /namespace ext


#endif


