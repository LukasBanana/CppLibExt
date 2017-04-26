/*
 * bit_mask.hpp file
 * 
 * Copyright (C) 2014-2017 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_BIT_MASK_H
#define CPPLIBEXT_BIT_MASK_H


#include <stdexcept>
#include <type_traits>


namespace ext
{


//! Base class for any bit mask (or rather options).
template <typename T>
class bit_mask
{
    
    public:
        
        static_assert(std::is_integral<T>::value, "bit_mask requires an integral type");

        using value_type    = T;
        using size_type     = std::size_t;

    private:
        
        struct num_bits
        {
            static const size_type value = sizeof(T)*8;
        };

    public:

        //! Bit iterator.
        class const_iterator
        {
            
            public:
                
                using value_type        = const T;
                using difference_type   = std::size_t;
                using pointer           = value_type*;
                using reference         = value_type&;
                using iterator_category = std::bidirectional_iterator_tag;

                const_iterator& operator ++ ()
                {
                    /* Move forwards as long as the next bit is true */
                    do
                    {
                        ++off_;
                    }
                    while (off_ < num_bits::value && not_set());
                    return *this;
                }

                const_iterator operator ++ (int) const
                {
                    auto result = *this;
                    operator ++ ();
                    return result;
                }

                const_iterator& operator -- ()
                {
                    /* Move forwards as long as the next bit is true */
                    do
                    {
                        --off_;
                    }
                    while (off_ > 0 && not_set());
                    return *this;
                }

                const_iterator operator -- (int) const
                {
                    auto result = *this;
                    operator -- ();
                    return result;
                }

                value_type operator * () const
                {
                    return T(1) << off_;
                }

                bool operator == (const const_iterator& rhs) const
                {
                    return &bits_ == &(rhs.bits_) && off_ == rhs.off_;
                }

                bool operator != (const const_iterator& rhs) const
                {
                    return !(*this == rhs);
                }

            protected:
                
                const_iterator(const T& bits, std::size_t off) :
                    bits_ { bits },
                    off_  { off  }
                {
                }

                bool not_set() const
                {
                    return ((bits_ >> off_) & T(1)) == 0;
                }

                friend class bit_mask;

            private:

                const T&    bits_;  //!< Reference to the bit mask.
                std::size_t off_;   //!< Bit offset.

        };

        bit_mask() = default;
        bit_mask(const value_type& bitMask) :
            bits_ { bitMask }
        {
        }

        //! Returns the maximal bit flag.
        value_type max() const
        {
            return T(1) << (num_bits::value - 1);
        }

        //! Returns the number of bits this mask can hold.
        size_type capacity() const
        {
            return num_bits::value;
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

        //! Returns a constant iterator to the first set bit.
        const_iterator begin() const
        {
            const_iterator it(bits_, 0);
            if (it.not_set())
                ++it;
            return it;
        }

        //! Returns a constant iterator after to the end of the bit mask.
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


