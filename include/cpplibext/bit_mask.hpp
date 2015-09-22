/*
 * bit_mask.hpp file
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef _CPPLIBEXT_BIT_MASK_H_
#define _CPPLIBEXT_BIT_MASK_H_


#include <stdexcept>


namespace ext
{


//! Base class for any bit mask (or rather options).
template <typename T> class bit_mask
{
    
    public:
        
        using value_type    = T;
        using size_type     = std::size_t;

    private:
        
        //! Bit iterator.
        template <typename U> class base_iterator
        {
            
            public:
                
                using value_type        = U;
                using difference_type   = size_t;
                using pointer           = value_type*;
                using reference         = value_type&;
                using iterator_category = std::bidirectional_iterator_tag;

                base_iterator& operator ++ ()
                {
                    /* Move forwards as long as the next bit is true */
                    do
                    {
                        ++off_;
                    }
                    while (((bits_ >> off_) & U(0x1)) == 0 && off_ < (sizeof(U)*8));
                    return *this;
                }
                base_iterator operator ++ (int) const
                {
                    auto result = *this;
                    operator ++ ();
                    return result;
                }

                base_iterator& operator -- ()
                {
                    /* Move forwards as long as the next bit is true */
                    do
                    {
                        --off_;
                    }
                    while (((bits_ >> off_) & U(0x1)) == 0 && off_ > 0);
                    return *this;
                }
                base_iterator operator -- (int) const
                {
                    auto result = *this;
                    operator -- ();
                    return result;
                }

                value_type operator * () const
                {
                    return U(1) << off_;
                }

                bool operator == (const base_iterator& rhs) const
                {
                    return &bits_ == &(rhs.bits_) && off_ == rhs.off_;
                }
                bool operator != (const base_iterator& rhs) const
                {
                    return !(*this == rhs);
                }

            private:
                
                base_iterator(U& bits, size_t off) :
                    bits_   ( bits ),
                    off_    ( off  )
                {
                }

                friend class bit_mask;

                U&      bits_;  //!< Reference to the bit mask.
                size_t  off_;   //!< Bit offset.

        };

    public:
        
        using iterator = base_iterator<T>;
        using const_iterator = base_iterator<T const>;

        bit_mask() = default;
        bit_mask(const value_type& bitMask) :
            bits_( bitMask )
        {
        }

        //! Returns the maximal bit flag.
        static value_type max()
        {
            return T(1) << (sizeof(T) * 8 - 1);
        }

        //! Returns true if the specified bit is set in this bit mask.
        bool find(const value_type& flag) const
        {
            return (bits_ & flag) != 0;
        }

        //! Adds the specifid bit flag.
        void insert(const value_type& flag)
        {
            bits_ |= flag;
        }

        //! Removes the specifid bit flag.
        void erase(const value_type& flag)
        {
            bits_ &= (~flag);
        }

        // \see find
        bool operator () (const value_type& flag) const
        {
            return find(flag);
        }

        // \see insert
        bit_mask& operator << (const value_type& flag)
        {
            insert(flag);
            return *this;
        }

        //! \see erase
        bit_mask& operator >> (const value_type& flag)
        {
            erase(flag);
            return *this;
        }

        //! Returns the bits of this bit mask.
        const value_type& data() const
        {
            return bits_;
        }

        //! \see bits
        operator const value_type& () const
        {
            return data();
        }

        iterator begin()
        {
            return iterator(bits_, 0);
        }
        iterator end()
        {
            return iterator(bits_, sizeof(T)*8);
        }

        const_iterator begin() const
        {
            return const_iterator(bits_, 0);
        }
        const_iterator end() const
        {
            return const_iterator(bits_, sizeof(T)*8);
        }

        /**
        \brief Returns the number of bits set to one.
        \remarks The complexity is O(n), where n is the number of bits 'value_type' has (i.e. n = sizeof(T)*8).
        */
        size_type size() const
        {
            size_type n = 0;
            for (auto it = begin(); it != end(); ++it)
                ++n;
            return n;
        }

    private:
        
        value_type bits_ = 0;

};


template <typename T> bool operator == (const bit_mask<T>& lhs, const bit_mask<T>& rhs)
{
    return lhs.data() == rhs.data();
}

template <typename T> bool operator != (const bit_mask<T>& lhs, const bit_mask<T>& rhs)
{
    return lhs.data() != rhs.data();
}


} // /namespace ext


#endif


