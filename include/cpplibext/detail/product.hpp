/*
 * product.hpp file
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_PRODUCT_H
#define CPPLIBEXT_PRODUCT_H


namespace ext
{

// This namespace is only used internally
namespace details
{


/*
meta template to compute the product of N inregral numbers (computes X1 * X2 * ... * XN):
product<int, 2, 5, 3>::value == 2*5*3 = 30;
*/

// Declaration for GCC and clang
template <typename T, T... XN>
struct product_secondary;

template <typename T, T X1, T... XN>
struct product
{
    // Brackets are required!
    static const T value = (X1 * product_secondary<T, XN...>::value);
};

template <typename T, T... XN>
struct product_secondary
{
    // Brackets are required!
    static const T value = (product<T, XN...>::value);
};

template <typename T>
struct product_secondary<T>
{
    static const T value = T(1);
};


} // /namespace details

} // /namespace ext


#endif


