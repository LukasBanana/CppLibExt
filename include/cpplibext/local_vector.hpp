/*
 * local_vector.hpp file
 *
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_LOCAL_VECTOR_H
#define CPPLIBEXT_LOCAL_VECTOR_H


#include <iterator>
#include <algorithm>
#include <utility>
#include <new>


namespace ext
{


template <typename T, std::size_t N>
class local_vector
{
    
        static_assert(N > 0, "size of local_vector must be greater than zero");
    
    public:
    
        using value_type                = T;
        using size_type                 = std::size_t;
        using difference_type           = std::ptrdiff_t;
        using reference                 = value_type&;
        using const_reference           = const value_type&;
        using pointer                   = value_type*;
        using const_pointer             = const value_type*;
        using iterator                  = pointer;
        using const_iterator            = const_pointer;
        using reverse_iterator          = std::reverse_iterator<iterator>;
        using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
    
    public:
    
        local_vector() = default;
    
        explicit local_vector(size_type count) :
            size_ { count }
        {
        }
    
        explicit local_vector(size_type count, const value_type& value) :
            size_ { count }
        {
            std::fill(std::begin(data_), std::begin(data_) + count, value);
        }
    
        local_vector(const local_vector& other) :
            size_ { other.size() }
        {
            std::copy(other.begin(), other.end(), begin());
        }
    
        local_vector(std::initializer_list<T> init) :
            size_ { init.size() }
        {
            std::copy(init.begin(), init.end(), begin());
        }
    
        /* ----- Element access ----- */
    
        reference front() noexcept
        {
            return data_[0];
        }
    
        const_reference front() const noexcept
        {
            return data_[0];
        }
    
        reference back() noexcept
        {
            return data_[size_ - 1];
        }
    
        const_reference back() const noexcept
        {
            return data_[size_ - 1];
        }

        pointer data() noexcept
        {
            return data_;
        }
    
        /* ----- Iterators ----- */
    
        iterator begin() noexcept
        {
            return data_;
        }
    
        const_iterator begin() const noexcept
        {
            return data_;
        }
    
        const_iterator cbegin() const noexcept
        {
            return data_;
        }
    
        iterator end() noexcept
        {
            return data_ + size_;
        }
    
        const_iterator end() const noexcept
        {
            return data_ + size_;
        }
    
        const_iterator cend() const noexcept
        {
            return data_ + size_;
        }
    
        reverse_iterator rbegin() noexcept
        {
            return reverse_iterator { end() };
        }
    
        const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator { end() };
        }
    
        const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator { cend() };
        }
    
        reverse_iterator rend() noexcept
        {
            return reverse_iterator { begin() };
        }
    
        const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator { begin() };
        }
    
        const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator { cbegin() };
        }

        /* ----- Capacity ----- */
    
        constexpr bool empty() const noexcept
        {
            return (size_ == 0);
        }
    
        constexpr size_type size() const noexcept
        {
            return size_;
        }
    
        constexpr size_type max_size() const noexcept
        {
            return N;
        }
    
        void reserve(size_type new_cap)
        {
            if (new_cap > N)
                throw std::bad_alloc();
        }
    
        constexpr size_type capacity() const noexcept
        {
            return N;
        }
    
        /* ----- Modifiers ----- */
    
        void clear()
        {
            size_ = 0;
        }
    
        iterator insert(const_iterator pos, const value_type& value)
        {
            assert_free_space();
            auto diff = pos - begin();
            
            /* Move everything after 'pos' to its right */
            for (size_type i = size(); i > diff; --i)
                data_[i] = data_[i - 1];
            
            /* Insert new element */
            data_[diff] = value;
            
            ++size_;
            
            return begin() + diff;
        }
    
        #if 0 //TODO
        void emplace();
        void erase();
        #endif
    
        void push_back(value_type&& value)
        {
            assert_free_space();
            data_[size_++] = std::forward<T>(value);
        }
    
        #if 0 //TODO
        void emplace_back();
        #endif
    
        void pop_back()
        {
            --size_;
        }
    
        void resize(size_type count)
        {
            if (count > N)
                throw std::bad_alloc();
            for (auto i = size_; i < count; ++i)
                data_[i] = T();
            size_ = count;
        }
    
        void resize(size_type count, const value_type& value)
        {
            if (count > N)
                throw std::bad_alloc();
            for (auto i = size_; i < count; ++i)
                data_[i] = value;
            size_ = count;
        }
    
    private:
    
        void assert_free_space()
        {
            if (size() == max_size())
                throw std::bad_alloc();
        }
    
    private:
    
        std::size_t size_       = 0;
        T           data_[N];
    
};


} // /namespace ext


#endif


