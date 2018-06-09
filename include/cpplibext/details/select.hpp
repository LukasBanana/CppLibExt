/*
 * select.hpp file
 * 
 * Copyright (C) 2014-2018 Lukas Hermanns
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
select<int, size_t(0), 2, 5, 3>::value == 2;
select<int, size_t(1), 2, 5, 3>::value == 5;
select<int, size_t(2), 2, 5, 3>::value == 3;
*/

// Declaration for GCC and clang
template <typename T, std::size_t index, T... XN>
struct select_secondary;

template <typename T, std::size_t index, T X1, T... XN>
struct select
{
    // Brackets are required!
    static const T value = (select_secondary<T, (index - std::size_t(1)), XN...>::value);
};

template <typename T, T X1, T... XN>
struct select<T, std::size_t(0), X1, XN...>
{
    static const T value = X1;
};

template <typename T, std::size_t index, T... XN>
struct select_secondary
{
    // Brackets are required!
    static const T value = (select<T, index, XN...>::value);
};


} // /namespace details

} // /namespace ext


#endif


