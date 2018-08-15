/*
 * growing_stack.hpp file
 *
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_GROWING_STACK_H
#define CPPLIBEXT_GROWING_STACK_H


#include <vector>
#include <algorithm>


namespace ext
{


template <class T, class Container = std::vector<T>>
class growing_stack
{

    public:

        using container_type    = Container;
        using value_type        = typename Container::value_type;
        using size_type         = typename Container::size_type;
        using reference         = typename Container::reference;
        using const_reference   = typename Container::const_reference;

    public:

        growing_stack() = default;
        growing_stack(const growing_stack&) = default;

        growing_stack(growing_stack&& rhs) :
            data_ { std::move(rhs) },
            size_ { rhs.size_ }
        {
            rhs.size_ = 0;
        }

        growing_stack& operator = (const growing_stack&) = default;

        growing_stack& operator = (growing_stack&& rhs)
        {
            data_ = std::move(rhs.data_);
            size_ = rhs.size_;
            rhs.size_ = 0;
            return *this;
        }

        reference top()
        {
            return data_[size_ - 1];
        }

        const_reference top() const
        {
            return data_[size_ - 1];
        }

        bool empty() const
        {
            return (size_ == 0);
        }

        size_type size() const
        {
            return size_;
        }

        void push(const value_type& value)
        {
            if (size_ < data_.size())
                data_[size_] = value;
            else
                data_.push_back(value);
            ++size_;
        }

        void push(value_type&& value)
        {
            if (size_ < data_.size())
                data_[size_] = std::move(value);
            else
                data_.push_back(std::forward<value_type&&>(value));
            ++size_;
        }

        /**
        \brief Removes the last element from the stack.
        \remarks If the stack is already empty, the behavior is undefined.
        */
        void pop()
        {
            if (size_ > 0)
                --size_;
        }

        void swap(growing_stack& other)
        {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
        }

    private:

        container_type  data_;
        size_type       size_ = 0;

};


} // /namespace ext


#endif


