/*
 * multi_array.hpp file
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <initializer_list>
#include <limits>
#include <algorithm>
#include <array>


// This namespace is only used internally
namespace detail
{

/* --- Product meta template (computes X1 * X2 * ... * XN) --- */

// Declaration for GCC and clang
template <typename T, T... XN> struct product_secondary;

template <typename T, T X1, T... XN> struct product
{
    // Brackets are required!
    static const T value = (X1 * product_secondary<T, XN...>::value);
};

template <typename T, T... XN> struct product_secondary
{
    // Brackets are required!
    static const T value = (product<T, XN...>::value);
};

template <typename T> struct product_secondary<T>
{
    static const T value = T(1);
};  

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


/**
Multi dimensional array class.
\tparam T Specifies the data type for the array elements.
\tparam Dimensions... Specifies the array dimensions. This must be at least 1 entry.
*/
template <typename T, std::size_t... Dimensions> class multi_array
{
    
    public:

        typedef T                   value_type;
        typedef std::size_t         size_type;
        typedef std::ptrdiff_t      difference_type;
        typedef value_type&         reference;
        typedef const value_type&   const_reference;
        typedef value_type*         pointer;
        typedef const value_type*   const_pointer;

    private:
        
        /*
        Helper structures with template meta programming
        to determine array size and size of slices
        */

        /* --- Array size (Dimension1 * Dimension2 * ... * DimensionN) --- */

        template <std::size_t... DimN> struct array_size
        {
            static const size_type value = detail::product<size_type, DimN...>::value;
        };

        /* --- Next array size (Dimension2 * Dimension3 * ... * DimensionN) --- */

        template <std::size_t FirstDimension, std::size_t... DimN> struct next_array_size
        {
            /* Ignore first dimension here, just use the variadic template arguments */
            static const size_type value = detail::product<size_type, DimN...>::value;
        };

        /* --- First dimension --- */

        template <std::size_t FirstDimension, std::size_t... DimN> struct first_dimension
        {
            /* Ignore variadic template arguments here, just use the first dimension */
            static const size_type value = FirstDimension;
        };

        /* --- next_dimension --- */

        // Declaration for GCC and clang
        template <std::size_t... DimN> struct next_dimension_secondary;

        template <std::size_t FirstDimension, std::size_t... DimN> struct next_dimension
        {
            static const size_type value = next_dimension_secondary<DimN...>::value;
        };

        template <std::size_t... DimN> struct next_dimension_secondary
        {
            static const size_type value = first_dimension<DimN...>::value;
        };

    public:
        
        //! Number of dimensions.
        static const size_type num_dimensions = sizeof...(Dimensions);

        //! Number of all elements in the array.
        static const size_type num_elements = array_size<Dimensions...>::value;

        //! Entire storage size (in bytes).
        static const size_type data_size = sizeof(value_type) * num_elements;

        //! Number of elements to the next slice.
        static const size_type stride = next_array_size<Dimensions...>::value;

        //! Iterator is just a pointer type (like in GCC 4.7)
        typedef value_type*                             iterator;
        typedef const value_type*                       const_iterator;

        //! TODO Reverse iterator does not negate the ++ and -- operators! (INCOMPLETE)
        typedef std::reverse_iterator<iterator>         reverse_iterator;
        typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

    private:

        typedef multi_array<T, Dimensions...> this_array_type;

        //! Array data storage.
        value_type data_[num_elements];

    public:

        multi_array()
        {
            static_assert(sizeof...(Dimensions) > 0, "multi_array must have at least 1 dimension");
        }

        pointer data()
        {
            return data_;
        }
        const_pointer data() const
        {
            return data_;
        }

        //! Returns the total number of elements.
        size_type size() const
        {
            return num_elements;
        }

        bool empty() const
        {
            return size() == 0;
        }

        //! Returns the maximal number of elements (equal to std::numeric_limits<size_type>::max()).
        size_type max_size() const
        {
            return std::numeric_limits<size_type>::max();
        }

        iterator begin()
        {
            return iterator(data());
        }
        const_iterator begin() const
        {
            return const_iterator(data());
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        iterator end()
        {
            return iterator(data() + num_elements);
        }
        const_iterator end() const
        {
            return const_iterator(data() + num_elements);
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        reference front()
        {
            return *begin();
        }
        const_reference front() const
        {
            return *begin();
        }

        reference back()
        {
            return num_elements > 0 ? *(end() - 1) : *end();
        }
        const_reference back() const
        {
            return num_elements > 0 ? *(end() - 1) : *end();
        }

        void fill(const value_type& value)
        {
            std::fill(begin(), end(), value);
        }

        void swap(this_array_type& other)
        {
            std::swap_ranges(begin(), end(), other.begin());
        }

        /**
        Returns the number of slices for the specifies dimension.
        \param[in] dimension Specifies the dimension index for the requested slices.
        \throws std::out_of_range If 'dimension' is greater then or equal to the number of array dimensions (num_dimensions).
        \see num_dimensions
        \remarks This is the dynamic version of "slices" which is slower than the static version.
        */
        size_type slices(const size_type& dimension) const
        {
            if (dimension >= num_dimensions)
                throw std::out_of_range("multi_array::slices out of range");
            std::array<size_type, num_dimensions> dim_list { Dimensions... };
            return dim_list[dimension];
        }

        /**
        Returns the number of slices for the specifies dimension.
        \tparam DimensionIndex Specifies the dimension index for the requested slices.
        \see num_dimensions
        \remarks This is the static version of "slices" which is faster than the dynamic version.
        */
        template <size_type DimensionIndex> size_type slices() const
        {
            static_assert(DimensionIndex < num_dimensions, "multi_array::slice out of range");
            return detail::select<size_type, DimensionIndex, Dimensions...>::value;
        }

        template <std::size_t CurrentDimension, std::size_t... NextDimensions> class slice
        {
            
            public:
                
                slice<NextDimensions...> operator [] (const size_type& index)
                {
                    return slice<NextDimensions...>(ptr_ + (multi_array<T, NextDimensions...>::stride * index));
                }

                slice<NextDimensions...> at(const size_type& index)
                {
                    if (index >= first_dimension<NextDimensions...>::value)
                        throw std::out_of_range("multi_array::slice out of range");
                    return slice<NextDimensions...>(ptr_ + (multi_array<T, NextDimensions...>::stride * index));
                }

                slice<CurrentDimension, NextDimensions...>& operator = (const value_type& value)
                {
                    std::fill(ptr_, ptr_ + multi_array<T, CurrentDimension, NextDimensions...>::stride, value);
                    return *this;
                }

            private:
                
                friend class multi_array;

                slice(pointer ptr) : ptr_(ptr)
                {
                }

                pointer ptr_ = nullptr;
                
        };

        template <std::size_t Dimension1, std::size_t Dimension2> class slice<Dimension1, Dimension2>
        {
            
            public:
                
                reference operator [] (const size_type& index)
                {
                    return ptr_[index];
                }

                reference at(const size_type& index)
                {
                    if (index >= Dimension2)
                        throw std::out_of_range("multi_array::slice out of range");
                    return ptr_[index];
                }

                slice<Dimension1, Dimension2>& operator = (const value_type& value)
                {
                    std::fill(ptr_, ptr_ + multi_array<T, Dimension1, Dimension2>::stride, value);
                    return *this;
                }

            private:
                
                friend class multi_array;

                slice(pointer ptr) : ptr_(ptr)
                {
                }

                pointer ptr_ = nullptr;
                
        };

        template <std::size_t CurrentDimension, std::size_t... NextDimensions> class const_slice
        {
            
            public:
                
                const_slice<NextDimensions...> operator [] (const size_type& index) const
                {
                    return const_slice<NextDimensions...>(ptr_ + (multi_array<T, NextDimensions...>::stride * index));
                }

                const_slice<NextDimensions...> at(const size_type& index) const
                {
                    if (index >= first_dimension<NextDimensions...>::value)
                        throw std::out_of_range("multi_array::const_slice out of range");
                    return const_slice<NextDimensions...>(ptr_ + (multi_array<T, NextDimensions...>::stride * index));
                }

            private:
                
                friend class multi_array;

                const_slice(const_pointer ptr) : ptr_(ptr)
                {
                }

                const_pointer ptr_ = nullptr;
                
        };

        template <std::size_t Dimension1, std::size_t Dimension2> class const_slice<Dimension1, Dimension2>
        {
            
            public:
                
                const_reference operator [] (const size_type& index) const
                {
                    return ptr_[index];
                }

                const_reference at(const size_type& index) const
                {
                    if (index >= Dimension2)
                        throw std::out_of_range("multi_array::const_slice out of range");
                    return ptr_[index];
                }

            private:
                
                friend class multi_array;

                const_slice(const_pointer ptr) : ptr_(ptr)
                {
                }

                const_pointer ptr_ = nullptr;
                
        };

        slice<Dimensions...> operator [] (const size_type& index)
        {
            return slice<Dimensions...>(data_ + (stride*index));
        }

        const_slice<Dimensions...> operator [] (const size_type& index) const
        {
            return slice<Dimensions...>(data_ + (stride*index));
        }

        slice<Dimensions...> at(const size_type& index)
        {
            if (index >= first_dimension<Dimensions...>::value)
                throw std::out_of_range("multi_array::slice out of range");
            return slice<Dimensions...>(data_ + (stride*index));
        }

        const_slice<Dimensions...> at(const size_type& index) const
        {
            if (index >= first_dimension<Dimensions...>::value)
                throw std::out_of_range("multi_array::slice out of range");
            return slice<Dimensions...>(data_ + (stride*index));
        }

};


/**
Multi dimensional array class.
\tparam T Specifies the data type for the array elements.
\remarks This is a template specialization of the multi-dimensional multi_array class.
*/
template <typename T, std::size_t Dimension> class multi_array<T, Dimension>
{
    
    public:

        typedef T                   value_type;
        typedef std::size_t         size_type;
        typedef std::ptrdiff_t      difference_type;
        typedef value_type&         reference;
        typedef const value_type&   const_reference;
        typedef value_type*         pointer;
        typedef const value_type*   const_pointer;

    public:
        
        //! Number of dimensions.
        static const size_type num_dimensions = 1;

        //! Number of all elements in the array.
        static const size_type num_elements = Dimension;

        //! Entire storage size (in bytes).
        static const size_type data_size = sizeof(value_type) * num_elements;

        //! Number of elements to the next slice.
        static const size_type stride = 1;

        typedef value_type*                             iterator;
        typedef const value_type*                       const_iterator;
        typedef std::reverse_iterator<iterator>         reverse_iterator;
        typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

    private:
        
        typedef multi_array<T, Dimension> this_array_type;

        //! Array data storage.
        value_type data_[num_elements];

    public:

        multi_array()
        {
        }

        pointer data()
        {
            return data_;
        }
        const_pointer data() const
        {
            return data_;
        }

        //! Returns the total number of elements.
        size_type size() const
        {
            return num_elements;
        }

        //! Returns the maximal number of elements (equal to std::numeric_limits<size_type>::max()).
        size_type max_size() const
        {
            return std::numeric_limits<size_type>::max();
        }

        bool empty() const
        {
            return size() == 0;
        }

        iterator begin()
        {
            return iterator(data());
        }
        const_iterator begin() const
        {
            return const_iterator(data());
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        iterator end()
        {
            return iterator(data() + num_elements);
        }
        const_iterator end() const
        {
            return const_iterator(data() + num_elements);
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        reference front()
        {
            return *begin();
        }
        const_reference front() const
        {
            return *begin();
        }

        reference back()
        {
            return num_elements > 0 ? *(end() - 1) : *end();
        }
        const_reference back() const
        {
            return num_elements > 0 ? *(end() - 1) : *end();
        }

        void fill(const value_type& value)
        {
            std::fill(begin(), end(), value);
        }

        void swap(this_array_type& other)
        {
            std::swap_ranges(begin(), end(), other.begin());
        }

        /**
        Returns the number of slices for the specifies dimension.
        \param[in] dimension Specifies the dimension index for the requested slices.
        \throws std::out_of_range If 'dimension' is greater then or equal to the number of array dimensions (num_dimensions).
        \see num_dimensions
        \remarks This is the dynamic version of "slices" which is slower than the static version.
        */
        size_type slices(const size_type& dimension) const
        {
            if (dimension >= num_dimensions)
                throw std::out_of_range("multi_array::slices out of range");
            return Dimension;
        }

        /**
        Returns the number of slices for the specifies dimension.
        \tparam DimensionIndex Specifies the dimension index for the requested slices.
        \see num_dimensions
        \remarks This is the static version of "slices" which is faster than the dynamic version.
        */
        template <size_type DimensionIndex> size_type slices() const
        {
            static_assert(DimensionIndex < num_dimensions, "multi_array::slice out of range");
            return Dimension;
        }

        reference operator [] (const size_type& index)
        {
            return data_[index];
        }

        const_reference operator [] (const size_type& index) const
        {
            return data_[index];
        }

        reference at(const size_type& index)
        {
            if (index >= num_elements)
                throw std::out_of_range("multi_array::slice out of range");
            return data_[index];
        }

        const_reference at(const size_type& index) const
        {
            if (index >= num_elements)
                throw std::out_of_range("multi_array::slice out of range");
            return data_[index];
        }

};



