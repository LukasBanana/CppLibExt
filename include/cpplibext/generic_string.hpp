/*
 * generic_string.hpp file
 *
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_GENERIC_STRING_H
#define CPPLIBEXT_GENERIC_STRING_H


#include "cstring_view.hpp"

#include <string>
#include <cstring>
#include <memory>
#include <cwchar>
#include <iterator>
#include <limits>
#include <algorithm>
#include <sstream>


namespace ext
{


//! \brief Basic generic string class that can be used as both mutable and immutable string (i.e. string view).
template <class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
class basic_generic_string
{

    public:

        using traits_type               = Traits;
        using value_type                = CharT;
        using pointer                   = CharT*;
        using const_pointer             = const CharT*;
        using reference                 = CharT&;
        using const_reference           = const CharT&;
        using iterator                  = CharT*;
        using const_iterator            = const CharT*;
        using reverse_iterator          = std::reverse_iterator<iterator>;
        using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
        using size_type                 = std::size_t;
        using difference_type           = std::ptrdiff_t;

    public:

        //! Equivalent to 'std::string::npos'.
        static constexpr size_type npos = size_type(-1);

    private:

        using immutable_string  = basic_cstring_view<CharT, Traits>;
        using mutable_string    = std::basic_string<CharT, Traits, Allocator>;

    public:

        /* ----- Constructors ----- */

        basic_generic_string() noexcept = default;
        basic_generic_string(const basic_generic_string& rhs) = default;

        //! Constructs this string view with the specified null terminated string.
        basic_generic_string(const CharT* s, bool copy = false)
        {
            if (copy)
                set_mutable(s);
            else
                set_immutable(s);
        }

        //! Constructs this string view with the specified null terminated string and its length (the length must be equal to '::strlen(str)').
        basic_generic_string(const CharT* str, std::size_t len, bool copy = true)
        {
            if (copy)
                set_mutable(str);
            else
                set_immutable(str);
        }

        //! Constructs this string view with the specified string.
        basic_generic_string(const std::basic_string<CharT, Traits, Allocator>& str, bool copy = true)
        {
            if (copy)
                set_mutable(str);
            else
                set_immutable(str);
        }

        /* ----- Operators ----- */

        basic_generic_string& operator = (const basic_generic_string& rhs)
        {

            return *this;
        }

        basic_generic_string& operator = (const std::basic_string<CharT, Traits, Allocator>& str)
        {
            set_mutable(str);
            return *this;
        }

        /* ----- Iterators ----- */

        //! Returns an iterator to the beginning.
        iterator begin()
        {
            return data();
        }

        //! Returns a constant iterator to the beginning.
        const_iterator begin() const
        {
            return data();
        }

        //! Returns an iterator to the beginning.
        const_iterator cbegin() const
        {
            return data();
        }

        //! Returns an iterator to the end.
        iterator end()
        {
            return data() + size();
        }

        //! Returns a constant iterator to the end.
        const_iterator end() const
        {
            return data() + size();
        }

        //! Returns an iterator to the end.
        const_iterator cend() const
        {
            return data() + size();
        }

        //! Returns a reverse iterator to the beginning.
        reverse_iterator rbegin()
        {
            return reverse_iterator { end() };
        }

        //! Returns a constant reverse iterator to the beginning.
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator { end() };
        }

        //! Returns a reverse iterator to the beginning.
        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator { end() };
        }

        //! Returns a reverse iterator to the end.
        reverse_iterator rend()
        {
            make_mutable();
            return reverse_iterator { begin() };
        }

        //! Returns a constant reverse iterator to the end.
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator { begin() };
        }

        //! Returns a reverse iterator to the end.
        const_reverse_iterator crend() const
        {
            return const_reverse_iterator { begin() };
        }

        /* ----- Element Access ----- */

        //! Accesses the element at the specified position without bounds check.
        reference operator [] (size_type pos)
        {
            make_mutable();
            return mutableStr_[pos];
        }

        //! Accesses the element at the specified position without bounds check.
        const_reference operator [] (size_type pos) const
        {
            return (mutable_ ? mutableStr_[pos] : immutableStr_[pos]);
        }

        /**
        \brief Accesses the element at the specified position with bounds check.
        \throws std::out_of_range If 'pos >= size()' holds true.
        */
        reference at(size_type pos)
        {
            make_mutable();
            return mutableStr_.at(pos);
        }

        /**
        \brief Accesses the element at the specified position with bounds check.
        \throws std::out_of_range If 'pos >= size()' holds true.
        */
        const_reference at(size_type pos) const
        {
            return (mutable_ ? mutableStr_.at(pos) : immutableStr_.at(pos));
        }

        //! Accesses the first element.
        reference front()
        {
            make_mutable();
            return mutableStr_.front();
        }

        //! Accesses the first element.
        const_reference front() const
        {
            return (mutable_ ? mutableStr_.front() : immutableStr_.front());
        }

        //! Accesses the last element.
        reference back()
        {
            make_mutable();
            return mutableStr_.back();
        }

        //! Accesses the last element.
        const_reference back() const
        {
            return (mutable_ ? mutableStr_.back() : immutableStr_.back());
        }

        //! Returns a pointer to the string data.
        pointer data()
        {
            make_mutable();
            return mutableStr_.data();
        }

        //! Returns a constant pointer to the string data.
        const_pointer data() const
        {
            return (mutable_ ? mutableStr_.data() : immutableStr_.data());
        }

        //! Returns a pointer to the null terminated string.
        const_pointer c_str() const
        {
            return (mutable_ ? mutableStr_.c_str() : immutableStr_.c_str());
        }

        /* ----- Capacity ----- */

        /**
        \breif Returns the number of characters.
        \remarks This is equivalent to the 'length' function.
        \see length
        */
        size_type size() const noexcept
        {
            return (mutable_ ? mutableStr_.size() : immutableStr_.size());
        }

        /**
        \breif Returns the number of characters.
        \remarks This is equivalent to the 'size' function.
        \see size
        */
        size_type length() const noexcept
        {
            return size();
        }

        //! Returns the maximum number of characters.
        size_type max_size() const noexcept
        {
            return (std::numeric_limits<size_type>::max() - 1);
        }

        //! Checks whether the view is empty.
        bool empty() const noexcept
        {
            return (mutable_ ? mutableStr_.empty() : immutableStr_.empty());
        }

        /* ----- Operations ----- */

        void clear()
        {
            mutableStr_.clear();
            mutable_ = true;
        }

        basic_generic_string& append(size_type count, CharT ch)
        {
            make_mutable();
            mutableStr_.append(count, ch);
            return *this;
        }

        basic_generic_string& append(const std::basic_string<CharT, Traits, Allocator>& str)
        {
            make_mutable();
            mutableStr_.append(str);
            return *this;
        }

        basic_generic_string& append(const std::basic_string<CharT, Traits, Allocator>& str, size_type pos, size_type count = npos)
        {
            make_mutable();
            mutableStr_.append(str, pos, count);
            return *this;
        }

        basic_generic_string& append(const CharT* s, size_type count)
        {
            make_mutable();
            mutableStr_.append(s, count);
            return *this;
        }

        basic_generic_string& append(const CharT* s)
        {
            make_mutable();
            mutableStr_.append(s);
            return *this;
        }

        /* ----- Comparison ----- */

        int compare(const basic_generic_string& v) const noexcept
        {
            return compare_primary(0, size(), v.c_str(), 0, v.size());
        }

        int compare(size_type pos1, size_type len1, const basic_generic_string& v) const noexcept
        {
            return compare_primary(pos1, len1, v.c_str(), 0, v.size());
        }

        int compare(size_type pos1, size_type len1, const basic_generic_string& v, size_type pos2, size_type len2) const noexcept
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

        /*size_type find(const basic_generic_string& v, size_type pos = 0) const noexcept
        {
            return find_primary(v.str_, pos, v.size());
        }

        size_type find(CharT ch, size_type pos = 0) const noexcept
        {
            return find_primary(&ch, pos, 1);
        }*/

        /* ----- Modifiers ----- */

        //! Exchanges the view with that of 'v'.
        void swap(basic_generic_string& v) noexcept
        {
            std::swap(immutableStr_, v.immutableStr_);
            std::swap(mutableStr_, v.mutableStr_);
            std::swap(mutable_, v.mutable_);
        }

        /* ----- Misc ----- */

        //! Makes this string mutable. Once the string is mutable, it cannot be made immutable again.
        void make_mutable()
        {
            if (!mutable_)
            {
                if (!immutableStr_.empty())
                    mutableStr_ = immutableStr_.c_str();
                mutable_ = true;
            }
        }

        //! Returns true, if this string is mutable. Otherwise, it only refers to a constant string pointer.
        bool is_mutable() const noexcept
        {
            return mutable_;
        }

    private:

        int compare_primary(size_type pos1, size_type len1, const CharT* s, size_type pos2, size_type len2) const noexcept
        {
            int result = Traits::compare(c_str() + pos1, s + pos2, std::min(len1, len2));
            if (result != 0)
                return result;

            if (len1 < len2)
                return -1;
            if (len1 > len2)
                return 1;

            return 0;
        }

        void set_mutable(const mutable_string& str)
        {
            mutableStr_ = str;
            mutable_    = true;
        }

        void set_immutable(const immutable_string& str)
        {
            if (mutable_)
            {
                if (auto s = str.c_str())
                    mutableStr_ = s;
            }
            else
                immutableStr_ = str;
        }

        immutable_string    immutableStr_;
        mutable_string      mutableStr_;
        bool                mutable_        = false;

};


/* ----- Comparison ----- */

template <class CharT, class Traits>
bool operator == (basic_generic_string<CharT, Traits> lhs, basic_generic_string<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) == 0);
}

template <class CharT, class Traits>
bool operator != (basic_generic_string<CharT, Traits> lhs, basic_generic_string<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) != 0);
}

template <class CharT, class Traits>
bool operator < (basic_generic_string<CharT, Traits> lhs, basic_generic_string<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) < 0);
}

template <class CharT, class Traits>
bool operator <= (basic_generic_string<CharT, Traits> lhs, basic_generic_string<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) <= 0);
}

template <class CharT, class Traits>
bool operator > (basic_generic_string<CharT, Traits> lhs, basic_generic_string<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) > 0);
}

template <class CharT, class Traits>
bool operator >= (basic_generic_string<CharT, Traits> lhs, basic_generic_string<CharT, Traits> rhs) noexcept
{
    return (lhs.compare(rhs) >= 0);
}


/* ----- Stream Output ----- */

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& os, basic_generic_string<CharT, Traits> v)
{
    if (auto s = v.c_str())
        os << s;
    return os;
}


using gstring = basic_generic_string<char>;
using wgstring = basic_generic_string<wchar_t>;
using u16gstring = basic_generic_string<char16_t>;
using u32gstring = basic_generic_string<char32_t>;


} // /namespace ext


#endif


