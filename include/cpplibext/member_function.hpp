/*
 * member_function.hpp file
 * 
 * Copyright (C) 2014-2019 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_MEMBER_FUNCTION_H
#define CPPLIBEXT_MEMBER_FUNCTION_H


#include <utility>


namespace ext
{



template <typename C, typename T>
class member_function { /* dummy */ };

//! Template for non-constant member functions with return type.
template <typename C, typename R, typename... Args>
class member_function<C, R(Args...)>
{

    public:
    
        typedef R (C::* pointer_type)(Args...);
    
    public:
    
        member_function(const member_function&) = default;
        member_function& operator = (const member_function&) = default;
    
        //! Initializes the member function with a null pointer.
        member_function()
        {
            ptr_.addr = nullptr;
        }
    
        //! Initializes the member function with the specified function pointer.
        member_function(pointer_type funcPtr)
        {
            ptr_.func = funcPtr;
        }
    
        //! Sets the function pointer to the specified member function.
        member_function& operator = (pointer_type funcPtr)
        {
            ptr_.func = funcPtr;
        }
    
        //! Calls the member function using the specified instance.
        R operator () (C& instance, Args&&... args) const
        {
            return ((instance).*(ptr_.func))(std::forward<Args>(args)...);
        }
    
        // Returns the function pointer that can be called.
        pointer_type get() const
        {
            return ptr_.func;
        }

        // Returns the raw pointer address of the member function.
        const void* ptr() const
        {
            return ptr_.addr;
        }
    
    private:
    
        union
        {
            pointer_type         func;
            const void* addr;
        }
        ptr_;
    
};

//! Template for non-constant member functions without return type.
template <typename C, typename... Args>
class member_function<C, void(Args...)>
{

    public:
    
        typedef void (C::* pointer_type)(Args...);
    
    public:
    
        member_function(const member_function&) = default;
        member_function& operator = (const member_function&) = default;
    
        //! Initializes the member function with a null pointer.
        member_function()
        {
            ptr_.addr = nullptr;
        }
    
        //! Initializes the member function with the specified function pointer.
        member_function(pointer_type funcPtr)
        {
            ptr_.func = funcPtr;
        }
    
        //! Sets the function pointer to the specified member function.
        member_function& operator = (pointer_type funcPtr)
        {
            ptr_.func = funcPtr;
        }
    
        //! Calls the member function using the specified instance.
        void operator () (C& instance, Args&&... args) const
        {
            ((instance).*(ptr_.func))(std::forward<Args>(args)...);
        }
    
        // Returns the function pointer that can be called.
        pointer_type get() const
        {
            return ptr_.func;
        }

        // Returns the raw pointer address of the member function.
        const void* ptr() const
        {
            return ptr_.addr;
        }
    
    private:
    
        union
        {
            pointer_type         func;
            const void* addr;
        }
        ptr_;
    
};

//! Template for constant member functions with return type.
template <typename C, typename R, typename... Args>
class member_function<C, R(Args...) const>
{

    public:
    
        typedef R (C::* pointer_type)(Args...) const;
    
    public:
    
        member_function(const member_function&) = default;
        member_function& operator = (const member_function&) = default;
    
        //! Initializes the member function with a null pointer.
        member_function()
        {
            ptr_.addr = nullptr;
        }
    
        //! Initializes the member function with the specified function pointer.
        member_function(pointer_type funcPtr)
        {
            ptr_.func = funcPtr;
        }
    
        //! Sets the function pointer to the specified member function.
        member_function& operator = (pointer_type funcPtr)
        {
            ptr_.func = funcPtr;
        }
    
        //! Calls the member function using the specified instance.
        R operator () (C& instance, Args&&... args) const
        {
            return ((instance).*(ptr_.func))(std::forward<Args>(args)...);
        }
    
        //! Returns the function pointer that can be called.
        pointer_type get() const
        {
            return ptr_.func;
        }

        //! Returns the raw pointer address of the member function.
        const void* ptr() const
        {
            return ptr_.addr;
        }
    
    private:
    
        union
        {
            pointer_type         func;
            const void* addr;
        }
        ptr_;
    
};

//! Template for constant member functions without return type.
template <typename C, typename... Args>
class member_function<C, void(Args...) const>
{

    public:
    
        typedef void (C::* pointer_type)(Args...) const;
    
    public:
    
        member_function(const member_function&) = default;
        member_function& operator = (const member_function&) = default;
    
        //! Initializes the member function with a null pointer.
        member_function()
        {
            ptr_.addr = nullptr;
        }
    
        //! Initializes the member function with the specified function pointer.
        member_function(pointer_type funcPtr)
        {
            ptr_.func = funcPtr;
        }
    
        //! Sets the function pointer to the specified member function.
        member_function& operator = (pointer_type funcPtr)
        {
            ptr_.func = funcPtr;
        }
    
        //! Calls the member function using the specified instance.
        void operator () (C& instance, Args&&... args) const
        {
            ((instance).*(ptr_.func))(std::forward<Args>(args)...);
        }
    
        // Returns the function pointer that can be called.
        pointer_type get() const
        {
            return ptr_.func;
        }

        // Returns the raw pointer address of the member function.
        const void* ptr() const
        {
            return ptr_.addr;
        }
    
    private:
    
        union
        {
            pointer_type    func;
            const void*     addr;
        }
        ptr_;
    
};



} // /namespace ext


#endif


