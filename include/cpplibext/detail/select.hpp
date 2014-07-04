/*
 * select.hpp file
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef _CPPLIBEXT_SELECT_H_
#define _CPPLIBEXT_SELECT_H_


#include <cstdlib>


// This namespace is only used internally
namespace detail
{

/* --- Select element from variadic template arguments --- */

// Declaration for GCC and clang
template <typename T, std::size_t index, T... XN> struct select_secondary;

template <typename T, std::size_t index, T X1, T... XN> struct select
{
    // Brackets are required!
    static const T value = (select_secondary<T, (index - std::size_t(1)), XN...>::value);
};

template <typename T, T X1, T... XN> struct select<T, std::size_t(0), X1, XN...>
{
    static const T value = X1;
};

template <typename T, std::size_t index, T... XN> struct select_secondary
{
    // Brackets are required!
    static const T value = (select<T, index, XN...>::value);
};

} // /namespace detail


#endif


