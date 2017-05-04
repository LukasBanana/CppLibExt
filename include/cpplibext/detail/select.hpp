/*
 * select.hpp file
 * 
 * Copyright (C) 2014-2017 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_SELECT_H
#define CPPLIBEXT_SELECT_H


#include <cstdlib>


namespace ext
{

// This namespace is only used internally
namespace details
{


/*
meta template to select entry of variadic template arguments:
select<int, 0, 2, 5, 3>::value == 2;
select<int, 1, 2, 5, 3>::value == 5;
select<int, 2, 2, 5, 3>::value == 3;
*/

    template <typename T, unsigned int index, T X1, T... XN>
    struct select
    {
        static constexpr T value = select<T, (index - 1), XN...>::value;
    };

    template <typename T, T X1, T... XN>
    struct select<T, 0, X1, XN...>
    {//when index decrease to zero, then successfull selected
        static constexpr T value = X1;
    };


} // /namespace details

} // /namespace ext


#endif


