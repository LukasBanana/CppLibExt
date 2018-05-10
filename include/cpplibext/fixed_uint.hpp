/*
 * fixed_uint.hpp file
 * 
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_FIXED_UINT_H
#define CPPLIBEXT_FIXED_UINT_H


#include <array>
#include <cstdint>
#include <type_traits>
#include <limits>
#include <algorithm>
#include <ostream>
#include <string>


namespace ext
{


/**
\brief Arithmetic class for fixed-size unsigned integers of big numbers.
\remarks Least significant elements (based on 'BaseType' template argument) are stored at the end of the internal buffer (Little Endian).
This does not define the endianess of the base types, only of the element arrangement inside the internal buffer.
*/
template <std::size_t BitSize, class BaseType = std::uint8_t>
class fixed_uint
{

        static_assert(std::is_unsigned<BaseType>::value, "base type of ext::fixed_uint must be an unsigned integral type");
        static_assert(BitSize % (sizeof(BaseType) * 8) == 0, "bit size of ext::fixed_uint must be a multiple of sizeof(BaseType)*8");

    public:

        using size_type = std::size_t;

    private:

        static const size_type element_size     = sizeof(BaseType);
        static const size_type element_bitsize  = element_size * 8;
        static const size_type num_elements     = BitSize / (element_size * 8);
        static const size_type buffer_size      = BitSize / 8;

    private:

        template <typename T>
        void convert_from(T rhs)
        {
            using min_type = std::conditional<sizeof(T) < sizeof(BaseType), T, BaseType>::type;

            const T min_mask = static_cast<T>(std::numeric_limits<min_type>::max());

            for (auto it = buffer_.rbegin(); it != buffer_.rend(); ++it)
            {
                if (rhs == 0)
                {
                    std::fill(it, buffer_.rend(), BaseType(0));
                    break;
                }
                else
                {
                    *it = (rhs & min_mask);
                    rhs >>= element_bitsize;
                }
            }
        }

        void reset()
        {
            std::fill(buffer_.begin(), buffer_.end(), BaseType(0));
        }

        void copy_from(const fixed_uint& rhs)
        {
            std::copy(rhs.buffer_.begin(), rhs.buffer_.end(), buffer_.begin());
        }

        void parse_from(const std::string& s)
        {
            //TODO...
        }

    public:

        fixed_uint()
        {
            reset();
        }

        fixed_uint(const fixed_uint& rhs)
        {
            copy_from(rhs);
        }

        fixed_uint(const std::string& rhs)
        {
            parse_from(rhs);
        }

        fixed_uint(std::uint8_t rhs)
        {
            convert_from(rhs);
        }

        fixed_uint(std::uint16_t rhs)
        {
            convert_from(rhs);
        }

        fixed_uint(std::uint32_t rhs)
        {
            convert_from(rhs);
        }

        fixed_uint(std::uint64_t rhs)
        {
            convert_from(rhs);
        }

    public:

        //! Converts this number into a string for the specified base (by default 10 for decimals).
        std::string str(size_type base = 10) const
        {
            std::string s;
            
            /* Construct digits from left to right (to efficiently push back new elements) */
            for (auto it = buffer_.rbegin(); it != buffer_.rend(); ++it)
            {
                auto v = *it;

                //TODO...
            }

            /* Correct output by reversing the string */
            std::reverse(s.begin(), s.end());

            return s;
        }

        //! Returns the internal buffer.
        const std::array<BaseType, num_elements>& data() const
        {
            return buffer_;
        }

        //! Returns the internal buffer.
        std::array<BaseType, num_elements>& data()
        {
            return buffer_;
        }

    public:

        fixed_uint& operator = (const fixed_uint& rhs)
        {
            copy_from(rhs);
            return *this;
        }

        fixed_uint& operator = (const std::string& rhs)
        {
            parse_from(rhs);
            return *this;
        }

        fixed_uint& operator += (const fixed_uint& rhs)
        {
            //TODO...
            return *this;
        }
        
        fixed_uint& operator -= (const fixed_uint& rhs)
        {
            //TODO...
            return *this;
        }
        
        fixed_uint& operator *= (const fixed_uint& rhs)
        {
            //TODO...
            return *this;
        }
        
        fixed_uint& operator /= (const fixed_uint& rhs)
        {
            //TODO...
            return *this;
        }

        fixed_uint& operator %= (const fixed_uint& rhs)
        {
            //TODO...
            return *this;
        }

        fixed_uint& operator <<= (const fixed_uint& rhs)
        {
            //TODO...
            return *this;
        }

        fixed_uint& operator >>= (const fixed_uint& rhs)
        {
            //TODO...
            return *this;
        }

        fixed_uint& operator &= (const fixed_uint& rhs)
        {
            //TODO...
            return *this;
        }

        fixed_uint& operator |= (const fixed_uint& rhs)
        {
            //TODO...
            return *this;
        }

        fixed_uint& operator ^= (const fixed_uint& rhs)
        {
            //TODO...
            return *this;
        }

        fixed_uint& operator ++ ()
        {
            //TODO...
            return *this;
        }
        
        fixed_uint operator ++ (int)
        {
            fixed_uint prev { this };
            ++(*this);
            return prev;
        }

        fixed_uint& operator -- ()
        {
            //TODO...
            return *this;
        }
        
        fixed_uint operator -- (int)
        {
            fixed_uint prev { this };
            --(*this);
            return prev;
        }

    private:

        std::array<BaseType, num_elements> buffer_;

};


template <std::size_t BitSize, class BaseType = std::uint8_t>
fixed_uint<BitSize, BaseType> operator + (const fixed_uint<BitSize, BaseType>& lhs, const fixed_uint<BitSize, BaseType>& rhs)
{
    fixed_uint<BitSize, BaseType> result { lhs };
    result += rhs;
    return result;
}

template <std::size_t BitSize, class BaseType = std::uint8_t>
fixed_uint<BitSize, BaseType> operator - (const fixed_uint<BitSize, BaseType>& lhs, const fixed_uint<BitSize, BaseType>& rhs)
{
    fixed_uint<BitSize, BaseType> result { lhs };
    result -= rhs;
    return result;
}

template <std::size_t BitSize, class BaseType = std::uint8_t>
fixed_uint<BitSize, BaseType> operator * (const fixed_uint<BitSize, BaseType>& lhs, const fixed_uint<BitSize, BaseType>& rhs)
{
    fixed_uint<BitSize, BaseType> result { lhs };
    result *= rhs;
    return result;
}

template <std::size_t BitSize, class BaseType = std::uint8_t>
fixed_uint<BitSize, BaseType> operator / (const fixed_uint<BitSize, BaseType>& lhs, const fixed_uint<BitSize, BaseType>& rhs)
{
    fixed_uint<BitSize, BaseType> result { lhs };
    result /= rhs;
    return result;
}

template <std::size_t BitSize, class BaseType = std::uint8_t>
fixed_uint<BitSize, BaseType> operator % (const fixed_uint<BitSize, BaseType>& lhs, const fixed_uint<BitSize, BaseType>& rhs)
{
    fixed_uint<BitSize, BaseType> result { lhs };
    result %= rhs;
    return result;
}

template <std::size_t BitSize, class BaseType = std::uint8_t>
fixed_uint<BitSize, BaseType> operator << (const fixed_uint<BitSize, BaseType>& lhs, const fixed_uint<BitSize, BaseType>& rhs)
{
    fixed_uint<BitSize, BaseType> result { lhs };
    result <<= rhs;
    return result;
}

template <std::size_t BitSize, class BaseType = std::uint8_t>
fixed_uint<BitSize, BaseType> operator >> (const fixed_uint<BitSize, BaseType>& lhs, const fixed_uint<BitSize, BaseType>& rhs)
{
    fixed_uint<BitSize, BaseType> result { lhs };
    result >>= rhs;
    return result;
}

template <std::size_t BitSize, class BaseType = std::uint8_t>
fixed_uint<BitSize, BaseType> operator & (const fixed_uint<BitSize, BaseType>& lhs, const fixed_uint<BitSize, BaseType>& rhs)
{
    fixed_uint<BitSize, BaseType> result { lhs };
    result &= rhs;
    return result;
}

template <std::size_t BitSize, class BaseType = std::uint8_t>
fixed_uint<BitSize, BaseType> operator | (const fixed_uint<BitSize, BaseType>& lhs, const fixed_uint<BitSize, BaseType>& rhs)
{
    fixed_uint<BitSize, BaseType> result { lhs };
    result |= rhs;
    return result;
}

template <std::size_t BitSize, class BaseType = std::uint8_t>
fixed_uint<BitSize, BaseType> operator ^ (const fixed_uint<BitSize, BaseType>& lhs, const fixed_uint<BitSize, BaseType>& rhs)
{
    fixed_uint<BitSize, BaseType> result { lhs };
    result ^= rhs;
    return result;
}

template <std::size_t BitSize, class BaseType = std::uint8_t>
std::ostream& operator << (std::ostream& stream, const fixed_uint<BitSize, BaseType>& num)
{
    /* Determine base for conversion from number-to-string */
    fixed_uint<BitSize, BaseType>::size_type base = 10;

    if ((stream.flags() & std::ios::hex) != 0)
        base = 16;
    else if ((stream.flags() & std::ios::oct) != 0)
        base = 8;

    /* Output number as string with determined base */
    stream << num.str(base);

    return stream;
}


using fixed_uint128 = fixed_uint<128, std::uint64_t>;
using fixed_uint256 = fixed_uint<256, std::uint64_t>;
using fixed_uint512 = fixed_uint<512, std::uint64_t>;
using fixed_uint1024 = fixed_uint<1024, std::uint64_t>;
using fixed_uint2048 = fixed_uint<2048, std::uint64_t>;
using fixed_uint4096 = fixed_uint<4096, std::uint64_t>;
using fixed_uint8192 = fixed_uint<8192, std::uint64_t>;


} // /namespace ext


#endif


