/*
 * path.hpp file
 * 
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_PATH_H
#define CPPLIBEXT_PATH_H


#include <string>
#include <stdexcept>


namespace ext
{


/**
\brief Directory path string class which consists of sub strings (one for each sub directory).
\remarks For example, a path "Foo/Bar" consists of the two sub strings "Foo" and "Bar".
This class always transforms '\\' characters into '/' characters.
*/
template <class StringT>
class basic_path
{

    public:

        using string_type   = StringT;
        using value_type    = typename StringT::value_type;
        using size_type     = typename StringT::size_type;

    public:

        class const_iterator
        {

            public:

                using this_type         = const_iterator;
                using char_type         = typename StringT::value_type;

                using iterator_category = std::bidirectional_iterator_tag;
                using value_type        = string_type;
                using difference_type   = typename string_type::difference_type;
                //using pointer           = typename iterator_type::pointer;
                //using reference         = typename iterator_type::reference;

            public:

                const_iterator() = default;
                const_iterator(const this_type&) = default;
                const_iterator(this_type&&) = default;

                const_iterator(const string_type* str, size_type pos) :
                    str_ { str },
                    pos_ { pos }
                {
                }

                this_type& operator = (const this_type&) = default;
                this_type& operator = (this_type&&) = default;

                bool operator == (const this_type& rhs) const
                {
                    return (str_ == rhs.str_ && pos_ == rhs.pos_);
                }

                bool operator != (const this_type& rhs) const
                {
                    return !(this->operator == (rhs));
                }

                this_type& operator ++ ()
                {
                    auto pos = str_->find(char_type('/'), pos_);
                    if (pos == string_type::npos)
                        pos_ = str_->size();
                    else
                        pos_ = pos + 1;
                    return *this;
                }

                this_type operator ++ (int)
                {
                    this_type prev { *this };
                    this->operator ++ ();
                    return prev;
                }

                this_type& operator -- ()
                {
                    if (pos_ >= 2)
                    {
                        auto pos = str_->find_last_of(char_type('/'), pos_ - 2);
                        if (pos == string_type::npos)
                            pos_ = 0;
                        else
                            pos_ = pos + 1;
                    }
                    else
                        pos_ = 0;
                    return *this;
                }

                this_type operator -- (int)
                {
                    this_type prev { *this };
                    this->operator -- ();
                    return prev;
                }

                value_type operator * () const
                {
                    if (!str_->empty())
                    {
                        auto posEnd = str_->find(char_type('/'), pos_);
                        if (posEnd == string_type::npos)
                            return str_->substr(pos_);
                        else
                            return str_->substr(pos_, posEnd + 1 - pos_);
                    }
                    return *str_;
                }

                //! Returns true if the sub path, pointed to by this iterator, refers to the "upper" directory.
                bool upper_dir() const
                {
                    if (pos_ + 2 <= str_->size())
                    {
                        const char_type compareStr[] = { char_type('.'), char_type('.'), char_type('/') };
                        auto posEnd = str_->find(char_type('/'), pos_);
                        if (posEnd == string_type::npos)
                            return equals(compareStr, 2, str_->size());
                        else
                            return equals(compareStr, 3, posEnd + 1);
                    }
                    return false;
                }

                //! Returns true if the sub path, pointed to by this iterator, refers to the "current" directory.
                bool current_dir() const
                {
                    if (pos_ + 1 <= str_->size())
                    {
                        const char_type compareStr[] = { char_type('.'), char_type('/') };
                        auto posEnd = str_->find(char_type('/'), pos_);
                        if (posEnd == string_type::npos)
                            return equals(compareStr, 1, str_->size());
                        else
                            return equals(compareStr, 2, posEnd + 1);
                    }
                    return false;
                }

            private:

                // Returns true if the sub path at the current iterator position equals the specified pattern of length 'len'.
                bool equals(const char_type* str, size_type len, size_type posEnd) const
                {
                    if (pos_ + len == posEnd)
                    {
                        for (auto pos = pos_; pos != posEnd; ++pos, ++str)
                        {
                            if ((*str_)[pos] != *str)
                                return false;
                        }
                        return true;
                    }
                    return false;
                }

                friend class basic_path;

                const string_type*  str_ = nullptr;
                size_type           pos_ = 0;

        };

    public:

        basic_path() = default;
        basic_path(const basic_path<StringT>&) = default;
        basic_path(basic_path<StringT>&&) = default;

        basic_path(const StringT& str) :
            str_ { str }
        {
            transform_path();
        }

        basic_path(const value_type* str) :
            str_ { str }
        {
            transform_path();
        }

        /**
        \brief Appends the specified sub path to this path.
        \throws std::invalid_argument If 'rhs' is an absolute path and this path is a non-empty relative path (see 'absolute()').
        */
        void push_back(const basic_path<StringT>& rhs)
        {
            insert(end(), rhs);
        }

        /**
        \brief Removes the last sub directory.
        \throws std::runtime_error If there is nothing left to remove.
        */
        void pop_back()
        {
            if (root())
            {
                /* Clear string */
                str_.clear();
            }
            else
            {
                /* Remove last sub path */
                auto pos = str_.find_last_of('/');
                if (pos != std::string::npos)
                    str_.erase(pos);
                else
                    throw std::runtime_error("cannot move further upwards in path");
            }
        }

        /**
        \brief Returns the number of sub directories (e.g. "/" is 1, "/Foo" is 2, "/Foo/Bar" is 3, but relatives paths like "Foo/Bar" is only 2).
        \remarks Complexity is O(n) where n is the number of characters of this path.
        */
        size_type size() const
        {
            if (!empty())
            {
                if (root())
                    return 1;

                size_type n = (absolute() ? 1 : 0);

                for (const auto& c : str_)
                {
                    if (c == value_type('/'))
                        ++n;
                }

                return n;
            }

            return 0;
        }

        //! Returns true if this path is the root level (i.e. "/", "A:/", "B:/", "C:/", etc.).
        bool root() const
        {
            return ( ( str_.size() == 1 && root_posix() ) || ( str_.size() == 3 && root_win32() ) );
        }

        //! Returns true if this is an absolute path, i.e. it begins at root level.
        bool absolute() const
        {
            return ( ( str_.size() >= 1 && root_posix() ) || ( str_.size() >= 3 && root_win32() ) );
        }

        //! Returns true if this path is empty.
        bool empty() const
        {
            return str_.empty();
        }

        //! Clears the entire path string.
        void clear()
        {
            str_.clear();
        }

        //! Returns the internal string value.
        const StringT& str() const
        {
            return str_;
        }

        //! Returns 'c_str()' on the internal string type.
        const value_type* c_str() const
        {
            return str_.c_str();
        }

    public:

        //! Returns a constant iterator to the beginning of this path.
        const_iterator begin() const
        {
            return const_iterator { &str_, 0u };
        }

        //! Returns a constant iterator to the end of this path.
        const_iterator end() const
        {
            return const_iterator { &str_, str_.size() };
        }

        //! Removes the sub path at the specified position.
        void erase(const const_iterator& pos)
        {
            auto posEnd = pos;
            ++posEnd;
            str_.erase(pos.pos_, posEnd.pos_ - pos.pos_);
        }

        //! Inserts the sub path after the specified position.
        void insert(const const_iterator& pos, const basic_path<StringT>& rhs)
        {
            if (empty())
            {
                /* Call copy operator, since this path is currently empty */
                this->operator = (rhs);
            }
            else
            {
                if (rhs.absolute())
                    throw std::invalid_argument("cannot insert absolute path into other path");

                if (pos.pos_ >= str_.size())
                    append_relative(rhs);
                else
                    insert_relative(pos, rhs);
            }
        }

    public:

        basic_path& operator = (const basic_path<StringT>&) = default;
        basic_path& operator = (basic_path<StringT>&&) = default;

        //! Appends the specified sub path to this path (see 'push_back').
        basic_path& operator += (const basic_path<StringT>& rhs)
        {
            push_back(rhs);
            return *this;
        }

        //! Implicit conversion to internal string value (see 'str()').
        operator const StringT& () const
        {
            return str();
        }

        //! Implicit conversion to pointer to null-terminated string of internal string value (see 'c_str()').
        operator const value_type* () const
        {
            return c_str();
        }

    private:

        void transform_path()
        {
            if (!empty())
            {
                /* Replace all '\\' characters by '/' */
                for (auto& c : str_)
                {
                    if (c == value_type('\\'))
                        c = value_type('/');
                }

                /* Ensure last character to be '/' for root, or remove for non-root */
                if (root())
                {
                    if (str_.back() != value_type('/'))
                        str_.push_back(value_type('/'));
                }
                else
                {
                    if (str_.back() == value_type('/'))
                        str_.pop_back();
                }

                /* Remove all redundant upper and current directories (i.e. "../" and "./" where possible) */
                erase_current_dirs();
                reduce_upper_dirs();
            }
        }

        // Remove all "current"-directory references (i.e. "./").
        void erase_current_dirs()
        {
            for (auto it = begin(); it != end(); ++it)
            {
                if (it.current_dir())
                    erase(it);
            }
        }

        // Removes unnecessary upper directories (e.g. "Foo/../Bar" -> "Bar").
        void reduce_upper_dirs()
        {
            if (!empty())
            {
                auto curr = begin();
                auto next = curr;
                ++next;

                while (next != end())
                {
                    if (!curr.upper_dir() && next.upper_dir())
                    {
                        /* Erase current and next sub path */
                        erase(next);
                        erase(curr);

                        /* Move one iteration back */
                        --curr;
                        next = curr;
                        ++next;
                    }
                    else
                    {
                        ++curr;
                        ++next;
                    }
                }
            }
        }

        void append_relative(const basic_path<StringT>& rhs)
        {
            /* Remove back sub paths for each appearance of "../" in the other path */
            auto it = rhs.begin();
            for (auto itEnd = rhs.end(); it != itEnd && it.upper_dir(); ++it)
                pop_back();

            /* Append new path */
            ensure_slash_end();
            if (it.pos_ == 0)
                str_ += rhs.str_;
            else
                str_ += rhs.str_.substr(it.pos_);

            /* Reduce redundant upper directories of resulting path */
            reduce_upper_dirs();
        }

        void insert_relative(const const_iterator& pos, const basic_path<StringT>& rhs)
        {
            //TODO...
        }

        bool root_posix() const
        {
            return (str_[0] == value_type('/'));
        }

        bool root_win32() const
        {
            if (str_[1] == value_type(':') && str_[2] == value_type('/'))
            {
                if (str_[0] >= value_type('a') && str_[0] <= value_type('z'))
                    return true;
                if (str_[0] >= value_type('A') && str_[0] <= value_type('Z'))
                    return true;
            }
            return false;
        }

        bool slash_end() const
        {
            return (!str_.empty() && str_.back() == value_type('/'));
        }

        void ensure_slash_end()
        {
            if (!slash_end())
                str_.push_back(value_type('/'));
        }

    private:

        StringT str_;

};

//! Concatenates the two paths.
template <class StringT>
basic_path<StringT> operator + (const basic_path<StringT>& lhs, const basic_path<StringT>& rhs)
{
    basic_path<StringT> result { lhs };
    result += rhs;
    return result;
}

template
<
    class StringT,
    class CharT,
    class Traits = std::char_traits<CharT>
>
std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& st, const basic_path<StringT>& p)
{
    st << p.str();
    return st;
}


using path = basic_path<std::string>;
using wpath = basic_path<std::wstring>;


} // /namespace ext


#endif


