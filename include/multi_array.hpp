
/*

multi_array.hpp
03/07/2014

Licensed under the terms of the GNU LGPLv3

Copyright (c) 2014  Lukas Hermanns

*/

/*

                   GNU LESSER GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.


  This version of the GNU Lesser General Public License incorporates
the terms and conditions of version 3 of the GNU General Public
License, supplemented by the additional permissions listed below.

  0. Additional Definitions.

  As used herein, "this License" refers to version 3 of the GNU Lesser
General Public License, and the "GNU GPL" refers to version 3 of the GNU
General Public License.

  "The Library" refers to a covered work governed by this License,
other than an Application or a Combined Work as defined below.

  An "Application" is any work that makes use of an interface provided
by the Library, but which is not otherwise based on the Library.
Defining a subclass of a class defined by the Library is deemed a mode
of using an interface provided by the Library.

  A "Combined Work" is a work produced by combining or linking an
Application with the Library.  The particular version of the Library
with which the Combined Work was made is also called the "Linked
Version".

  The "Minimal Corresponding Source" for a Combined Work means the
Corresponding Source for the Combined Work, excluding any source code
for portions of the Combined Work that, considered in isolation, are
based on the Application, and not on the Linked Version.

  The "Corresponding Application Code" for a Combined Work means the
object code and/or source code for the Application, including any data
and utility programs needed for reproducing the Combined Work from the
Application, but excluding the System Libraries of the Combined Work.

  1. Exception to Section 3 of the GNU GPL.

  You may convey a covered work under sections 3 and 4 of this License
without being bound by section 3 of the GNU GPL.

  2. Conveying Modified Versions.

  If you modify a copy of the Library, and, in your modifications, a
facility refers to a function or data to be supplied by an Application
that uses the facility (other than as an argument passed when the
facility is invoked), then you may convey a copy of the modified
version:

   a) under this License, provided that you make a good faith effort to
   ensure that, in the event an Application does not supply the
   function or data, the facility still operates, and performs
   whatever part of its purpose remains meaningful, or

   b) under the GNU GPL, with none of the additional permissions of
   this License applicable to that copy.

  3. Object Code Incorporating Material from Library Header Files.

  The object code form of an Application may incorporate material from
a header file that is part of the Library.  You may convey such object
code under terms of your choice, provided that, if the incorporated
material is not limited to numerical parameters, data structure
layouts and accessors, or small macros, inline functions and templates
(ten or fewer lines in length), you do both of the following:

   a) Give prominent notice with each copy of the object code that the
   Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the object code with a copy of the GNU GPL and this license
   document.

  4. Combined Works.

  You may convey a Combined Work under terms of your choice that,
taken together, effectively do not restrict modification of the
portions of the Library contained in the Combined Work and reverse
engineering for debugging such modifications, if you also do each of
the following:

   a) Give prominent notice with each copy of the Combined Work that
   the Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the Combined Work with a copy of the GNU GPL and this license
   document.

   c) For a Combined Work that displays copyright notices during
   execution, include the copyright notice for the Library among
   these notices, as well as a reference directing the user to the
   copies of the GNU GPL and this license document.

   d) Do one of the following:

       0) Convey the Minimal Corresponding Source under the terms of this
       License, and the Corresponding Application Code in a form
       suitable for, and under terms that permit, the user to
       recombine or relink the Application with a modified version of
       the Linked Version to produce a modified Combined Work, in the
       manner specified by section 6 of the GNU GPL for conveying
       Corresponding Source.

       1) Use a suitable shared library mechanism for linking with the
       Library.  A suitable mechanism is one that (a) uses at run time
       a copy of the Library already present on the user's computer
       system, and (b) will operate properly with a modified version
       of the Library that is interface-compatible with the Linked
       Version.

   e) Provide Installation Information, but only if you would otherwise
   be required to provide such information under section 6 of the
   GNU GPL, and only to the extent that such information is
   necessary to install and execute a modified version of the
   Combined Work produced by recombining or relinking the
   Application with a modified version of the Linked Version. (If
   you use option 4d0, the Installation Information must accompany
   the Minimal Corresponding Source and Corresponding Application
   Code. If you use option 4d1, you must provide the Installation
   Information in the manner specified by section 6 of the GNU GPL
   for conveying Corresponding Source.)

  5. Combined Libraries.

  You may place library facilities that are a work based on the
Library side by side in a single library together with other library
facilities that are not Applications and are not covered by this
License, and convey such a combined library under terms of your
choice, if you do both of the following:

   a) Accompany the combined library with a copy of the same work based
   on the Library, uncombined with any other library facilities,
   conveyed under the terms of this License.

   b) Give prominent notice with the combined library that part of it
   is a work based on the Library, and explaining where to find the
   accompanying uncombined form of the same work.

  6. Revised Versions of the GNU Lesser General Public License.

  The Free Software Foundation may publish revised and/or new versions
of the GNU Lesser General Public License from time to time. Such new
versions will be similar in spirit to the present version, but may
differ in detail to address new problems or concerns.

  Each version is given a distinguishing version number. If the
Library as you received it specifies that a certain numbered version
of the GNU Lesser General Public License "or any later version"
applies to it, you have the option of following the terms and
conditions either of that published version or of any later version
published by the Free Software Foundation. If the Library as you
received it does not specify a version number of the GNU Lesser
General Public License, you may choose any version of the GNU Lesser
General Public License ever published by the Free Software Foundation.

  If the Library as you received it specifies that a proxy can decide
whether future versions of the GNU Lesser General Public License shall
apply, that proxy's public statement of acceptance of any version is
permanent authorization for you to choose that version for the
Library.

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



