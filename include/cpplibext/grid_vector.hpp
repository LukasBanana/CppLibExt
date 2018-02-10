/*
 * grid_vector.hpp file
 * 
 * Copyright (C) 2014-2017 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_GRID_VECTOR_H
#define CPPLIBEXT_GRID_VECTOR_H


#include <vector>


namespace ext
{


template <typename T, class Alloc = std::allocator<T>>
class grid_vector
{
    
    private:

        /* --- Extended types --- */
        using storage_type  = std::vector<T, Alloc>;
        using this_type     = grid_vector<T, Alloc>;

    public:

        /* --- std::vector types --- */
        using value_type                = typename storage_type::value_type;
        using allocator_type            = typename storage_type::allocator_type;
        using pointer                   = typename storage_type::pointer;
        using const_pointer             = typename storage_type::const_pointer;
        using reference                 = typename storage_type::reference;
        using const_reference           = typename storage_type::const_reference;
        using iterator                  = typename storage_type::iterator                 ;
        using const_iterator            = typename storage_type::const_iterator;
        using reverse_iterator          = typename storage_type::reverse_iterator;
        using const_reverse_iterator    = typename storage_type::const_reverse_iterator;
        using difference_type           = typename storage_type::difference_type;
        using size_type                 = typename storage_type::size_type;

    private:

        //! Vector data storage.
        storage_type data_;

        size_type width_ = 0, height_ = 0;

    public:

        grid_vector() = default;
        grid_vector(this_type&& other) :
            data_   { std::move(other.data_) },
            width_  { other.width_           },
            height_ { other.height_          }
        {
        }

        void resize(size_type width, size_type height)
        {
            width_ = width;
            height_ = height;
            data_.resize(width*height);
        }
        void resize(size_type width, size_type height, const value_type& val)
        {
            width_ = width;
            height_ = height;
            data_.resize(width*height, val);
        }

        size_type width() const
        {
            return width_;
        }
        size_type height() const
        {
            return height_;
        }

        reference operator () (size_t x, size_t y)
        {
            return data_[y*width()+x];
        }
        const_reference operator () (size_t x, size_t y) const
        {
            return data_[y*width()+x];
        }

        reference at(size_t x, size_t y)
        {
            return data_.at(y*width()+x);
        }
        const_reference at(size_t x, size_t y) const
        {
            return data_.at(y*width()+x);
        }

        value_type* data()
        {
            return data_.data();
        }
        const value_type* data() const
        {
            return data_.data();
        }

        bool empty() const
        {
            return data_.empty();
        }

        iterator begin()
        {
            return data_.begin();
        }
        iterator end()
        {
            return data_.end();
        }

        const_iterator begin() const
        {
            return data_.begin();
        }
        const_iterator end() const
        {
            return data_.end();
        }

        reverse_iterator rbegin()
        {
            return data_.rbegin();
        }
        reverse_iterator rend()
        {
            return data_.rend();
        }

        const_reverse_iterator rbegin() const
        {
            return data_.rbegin();
        }
        const_reverse_iterator rend() const
        {
            return data_.rend();
        }

};


} // /namespace ext


#endif


