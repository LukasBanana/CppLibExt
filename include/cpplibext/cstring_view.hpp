/*
 * cstring_view.hpp file
 *
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_CSTRING_VIEW_H
#define CPPLIBEXT_CSTRING_VIEW_H


#include "cstring_len.hpp"

#include <string>
#include <cstring>
#include <cwchar>
#include <iterator>
#include <limits>
#include <algorithm>
#include <sstream>


namespace ext
{


/**
\brief Basic string view class.
\remarks This class is NOT compatible to the C++17 class 'std::basic_string_view',
since this class (i.e. 'ext::basic_cstring_view') expects the wrapped string pointer to have a null terminator.
*/
template <class CharT, class Traits = std::char_traits<CharT>>
class basic_cstring_view
{

    public:

        using traits_type               = Traits;
        using value_type                = CharT;
        using pointer                   = CharT*;
        using const_pointer             = const CharT*;
        using reference                 = CharT&;
        using const_reference           = const CharT&;
        using const_iterator            = const CharT*;
        using iterator                  = const_iterator;
        using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
        using reverse_iterator          = const_reverse_iterator;
        using size_type                 = std::size_t;
        using difference_type           = std::ptrdiff_t;

    public:

        //! Equivalent to 'std::string::npos'.
        static constexpr size_type npos = size_type(-1);

    public:

        /* ----- Constructors ----- */

        constexpr basic_cstring_view() noexcept = default;
        constexpr basic_cstring_view(const basic_cstring_view&) noexcept = default;

        //! Constructs this string view with the specified null terminated string.
        basic_cstring_view(const CharT* s) :
            str_ { s              },
            len_ { cstring_len(s) }
        {
        }

        //! Constructs this string view with the specified null terminated string and its length (the length must be equal to '::strlen(str)').
        constexpr basic_cstring_view(const CharT* str, std::size_t len) :
            str_ { str },
            len_ { len }
        {
        }

        //! Constructs this string view with the specified string.
        template <class Alloc>
        constexpr basic_cstring_view(const std::basic_string<CharT, Traits, Alloc>& str) :
            str_ { str.c_str() },
            len_ { str.size()  }
        {
        }

        /* ----- Operators ----- */

        basic_cstring_view& operator = (const basic_cstring_view&) noexcept = default;

        template <class Alloc>
        basic_cstring_view& operator = (const std::basic_string<CharT, Traits, Alloc>& str)
        {
            str_ = str.c_str();
            len_ = str.size();
            return *this;
        }

        /* ----- Iterators ----- */

        //! Returns an iterator to the beginning.
        constexpr const_iterator begin() const
        {
            return str_;
        }

        //! Returns an iterator to the beginning.
        constexpr const_iterator cbegin() const
        {
            return str_;
        }

        //! Returns an iterator to the end.
        constexpr const_iterator end() const
        {
            return str_ + len_;
        }

        //! Returns an iterator to the end.
        constexpr const_iterator cend() const
        {
            return str_ + len_;
        }

        //! Returns an reverse iterator to the beginning.
        constexpr const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator { end() };
        }

        //! Returns an reverse iterator to the beginning.
        constexpr const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator { end() };
        }

        //! Returns an reverse iterator to the end.
        constexpr const_reverse_iterator rend() const
        {
            return const_reverse_iterator { begin() };
        }

        //! Returns an reverse iterator to the end.
        constexpr const_reverse_iterator crend() const
        {
            return const_reverse_iterator { begin() };
        }

        /* ----- Element Access ----- */

        //! Accesses the element at the specified position without bounds check.
        constexpr const_reference operator [] (size_type pos) const
        {
            return str_[pos];
        }

        /**
        \brief Accesses the element at the specified position with bounds check.
        \throws std::out_of_range If 'pos >= size()' holds true.
        */
        const_reference at(size_type pos) const
        {
            if (pos >= size())
                throw std::out_of_range("'pos' is out of range in string view");
            return this->operator [] (pos);
        }

        //! Accesses the first element.
        constexpr const_reference front() const
        {
            return this->operator[](0);
        }

        //! Accesses the last element.
        constexpr const_reference back() const
        {
            return this->operator[](size() - 1);
        }

        //! Returns a pointer to the string data.
        constexpr const_pointer data() const
        {
            return str_;
        }

        //! Returns a pointer to the null terminated string.
        constexpr const_pointer c_str() const
        {
            return str_;
        }

        /* ----- Capacity ----- */

        /**
        \breif Returns the number of characters.
        \remarks This is equivalent to the 'length' function.
        \see length
        */
        constexpr size_type size() const noexcept
        {
            return len_;
        }

        /**
        \breif Returns the number of characters.
        \remarks This is equivalent to the 'size' function.
        \see size
        */
        constexpr size_type length() const noexcept
        {
            return len_;
        }

        //! Returns the maximum number of characters.
        constexpr size_type max_size() const noexcept
        {
            return (std::numeric_limits<size_type>::max() - 1);
        }

        //! Checks whether the view is empty.
        constexpr bool empty() const noexcept
        {
            return (len_ == 0);
        }

        /* ----- Comparison ----- */

        int compare(const basic_cstring_view& v) const noexcept
        {
            return compare_primary(0, size(), v.c_str(), 0, v.size());
        }

        int compare(size_type pos1, size_type len1, const basic_cstring_view& v) const noexcept
        {
            return compare_primary(pos1, len1, v.c_str(), 0, v.size());
        }

        int compare(size_type pos1, size_type len1, const basic_cstring_view& v, size_type pos2, size_type len2) const noexcept
        {
            return compare_primary(pos1, len1, v.c_str(), pos2, len2);
        }

        int compare(const CharT* s) const noexcept
        {
            return compare_primary(0, size(), s, 0, cstring_len(s));
        }

        int compare(size_type pos1, size_type len1, const CharT* s) const noexcept
        {
            return compare_primary(pos1, len1, s, 0, cstring_len(s));
        }

        int compare(size_type pos1, size_type len1, const CharT* s, size_type pos2, size_type len2) const noexcept
        {
            return compare_primary(pos1, len1, s, pos2, len2);
        }

        /* ----- Searching ----- */

        /*size_type find(const basic_cstring_view& v, size_type pos = 0) const noexcept
        {
            return find_primary(v.str_, pos, v.size());
        }

        size_type find(CharT ch, size_type pos = 0) const noexcept
        {
            return find_primary(&ch, pos, 1);
        }*/

        /* ----- Modifiers ----- */

        //! Exchanges the view with that of 'v'.
        void swap(basic_cstring_view& v) noexcept
        {
            std::swap(str_, v.str_);
            std::swap(len_, v.len_);
        }

    private:

        int compare_primary(size_type pos1, size_type len1, const CharT* s, size_type pos2, size_type len2) const noexcept
        {
            int result = Traits::compare(str_ + pos1, s + pos2, std::min(len1, len2));
            if (result != 0)
                return result;

            if (len1 < len2)
                return -1;
            if (len1 > len2)
                return 1;

            return 0;
        }

        /*size_type find_primary(const CharT* s, size_type pos, size_type len) const noexcept
        {
            if (auto off = Traits::find(str_, len_))
            {

            }
            return npos;
        }*/

        const_pointer   str_ = nullptr;
        size_type       len_ = 0;

};


/* ----- Comparison ----- */

template <class CharT, class Traits>
bool operator == (basic_cstring_view<CharT, Traits> lhs, basic_cstring_view<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) == 0);
}

template <class CharT, class Traits>
bool operator != (basic_cstring_view<CharT, Traits> lhs, basic_cstring_view<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) != 0);
}

template <class CharT, class Traits>
bool operator < (basic_cstring_view<CharT, Traits> lhs, basic_cstring_view<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) < 0);
}

template <class CharT, class Traits>
bool operator <= (basic_cstring_view<CharT, Traits> lhs, basic_cstring_view<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) <= 0);
}

template <class CharT, class Traits>
bool operator > (basic_cstring_view<CharT, Traits> lhs, basic_cstring_view<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) > 0);
}

template <class CharT, class Traits>
bool operator >= (basic_cstring_view<CharT, Traits> lhs, basic_cstring_view<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) >= 0);
}


/* ----- Stream Output ----- */

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& os, basic_cstring_view<CharT, Traits> v)
{
    if (auto s = v.c_str())
        os << s;
    return os;
}


using cstring_view = basic_cstring_view<char>;
using wcstring_view = basic_cstring_view<wchar_t>;
using u16cstring_view = basic_cstring_view<char16_t>;
using u32cstring_view = basic_cstring_view<char32_t>;


} // /namespace ext


#endif


