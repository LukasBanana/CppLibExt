/*
 * make_unique.hpp file
 * 
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_MAKE_UNIQUE_H
#define CPPLIBEXT_MAKE_UNIQUE_H


#include <memory>


namespace ext
{


//! Alternative for 'std::make_unique' for C++11 compilers, that do not support C++14.
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


} // /namespace ext


#endif


