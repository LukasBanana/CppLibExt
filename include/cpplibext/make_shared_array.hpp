/*
 * make_shared_array.hpp file
 * 
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_MAKE_ARRAY_H
#define CPPLIBEXT_MAKE_ARRAY_H


#include <memory>


namespace ext
{


/**
\brief Makes a new shared_ptr of type 'T[size]' and sets a custom deleter to allow array pointers.
\tparam T Specifies the pointer type.
\param[in] size Specifies the array size.
*/
template <class T>
std::shared_ptr<T> make_shared_array(const size_t size)
{
    /* Allocate shared memory manually and set custom deleter to allow array pointers */
    return std::shared_ptr<T>(
        new T[size],
        [](T* ptr)
        {
            delete [] ptr;
        }
    );
}


} // /namespace ext


#endif


