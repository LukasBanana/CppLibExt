/*
 * packet_vector.hpp file
 * 
 * Copyright (C) 2014-2017 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CPPLIBEXT_PACKED_VECTOR_H
#define CPPLIBEXT_PACKED_VECTOR_H


#include <vector>
#include <type_traits>
#include <algorithm>
#include <exception>
#include <iterator>


namespace ext
{


/**
Packed vector class.
\tparam Base Specifies the base class for all elements in the packed vector.
\code
// Example usage:
struct A
{
    virtual ~A()
    {
    }
    int x = 1;
};
struct B : public A
{
    int y = 2;
};
struct C
{
    int z = 0;
};

ext::packed_vector<A> list;

list.push_back(A());
list.push_back(B());
list.push_back(B());

auto a = list.get<A>(0);
auto b = list.get<B>(1);
//auto c = list.get<C>(2); // 2 errors: 3rd element is of type 'B' and 'C' is not a derived type of 'A'.
\endcode
\todo Copy constructor is NOT called within a "push_back" or "insert"!
*/
template <typename Base>
class packed_vector
{
    
    public:
        
        /* --- Type definitions --- */

        typedef std::vector<char>           storage_type;
        typedef storage_type::size_type     size_type;

    private:
        
        //! Structure to store meta data for each element in the data storage.
        struct element
        {
            size_type offset;       //!< Offset (in bytes) where the element starts.
            size_type size;         //!< Size of this element (in bytes).
            size_type base_offset;  //!< Base offset (in bytes) for the 'Base' type.
        };

        typedef std::vector<element> chart_type;

        /* --- Members --- */

        chart_type      chart_; //!< Element offset chart.
        storage_type    data_;  //!< Data storage.

        /* --- Functions --- */

        template <typename Target> size_type get_base_offset(const Target& val)
        {
            assert_target_type<Target>();
            const auto basePtr = static_cast<const Base*>(&val);
            return reinterpret_cast<size_type>(basePtr) - reinterpret_cast<size_type>(&val);
        }

        /* --- Iterators --- */

        /**
        Base iterator class for packed_vector.
        \tparam BaseType Must be either 'Base' or 'const Base'.
        \tparam ChartType Must be either 'chart_type' or 'const chart_type'.
        \tparam StorageType Must be either 'storage_type' or 'const storage_type'.
        */
        template <typename BaseType, typename ChartType, typename StorageType>
        class base_iterator : public std::iterator<std::random_access_iterator_tag, BaseType>
        {
            
            private:
                
                typedef base_iterator<BaseType, ChartType, StorageType> this_type;
            
            public:
                
                using reference = typename std::iterator<std::random_access_iterator_tag, BaseType>::reference;
                using pointer = typename std::iterator<std::random_access_iterator_tag, BaseType>::pointer;
                using difference_type = typename std::iterator<std::random_access_iterator_tag, BaseType>::difference_type;
            
                base_iterator() = default;

                /* --- Comparision operators --- */

                bool operator == (const this_type& other) const
                {
                    return
                        pos_        == other.pos_       &&
                        chartRef_   == other.chartRef_  &&
                        dataRef_    == other.dataRef_;
                }
                bool operator != (const this_type& other) const
                {
                    return !(*this == other);
                }

                bool operator < (const this_type& other) const
                {
                    return
                        pos_        <  other.pos_       &&
                        chartRef_   == other.chartRef_  &&
                        dataRef_    == other.dataRef_;
                }
                bool operator <= (const this_type& other) const
                {
                    return
                        pos_        <=  other.pos_      &&
                        chartRef_   == other.chartRef_  &&
                        dataRef_    == other.dataRef_;
                }

                bool operator > (const this_type& other) const
                {
                    return
                        pos_        >  other.pos_       &&
                        chartRef_   == other.chartRef_  &&
                        dataRef_    == other.dataRef_;
                }
                bool operator >= (const this_type& other) const
                {
                    return
                        pos_        >= other.pos_       &&
                        chartRef_   == other.chartRef_  &&
                        dataRef_    == other.dataRef_;
                }
            
                /* --- Access operators --- */

                reference operator * () const
                {
                    auto entry = chartRef_->at(pos_);
                    return *reinterpret_cast<BaseType*>(&(*dataRef_)[entry.offset + entry.base_offset]);
                }
                pointer operator -> () const
                {
                    auto entry = chartRef_->at(pos_);
                    return reinterpret_cast<BaseType*>(&(*dataRef_)[entry.offset + entry.base_offset]);
                }

                reference operator [] (const difference_type& offset) const
                {
                    return *(operator + (offset));
                }

                /* --- Iteration operators --- */

                this_type& operator ++ ()
                {
                    ++pos_;
                    return *this;
                }
                this_type operator ++ (int)
                {
                    auto tmp(*this);
                    operator ++ ();
                    return this;
                }

                this_type& operator -- ()
                {
                    --pos_;
                    return *this;
                }
                this_type operator -- (int)
                {
                    auto tmp(*this);
                    operator -- ();
                    return tmp;
                }

                this_type& operator += (const difference_type& offset)
                {
                    pos_ += offset;
                    return *this;
                }
                this_type operator + (const difference_type& offset) const
                {
                    auto tmp(*this);
                    tmp.pos_ += offset;
                    return tmp;
                }

                this_type& operator -= (const difference_type& offset)
                {
                    pos_ -= offset;
                    return *this;
                }
                this_type operator - (const difference_type& offset) const
                {
                    auto tmp(*this);
                    tmp.pos_ -= offset;
                    return tmp;
                }

                /* --- Conversion operators --- */

                operator base_iterator<
                    typename std::add_const<BaseType>::type,
                    typename std::add_const<ChartType>::type,
                    typename std::add_const<StorageType>::type> () const
                {
                    return { pos_, chartRef_, dataRef_ };
                }

                /* --- Functions --- */

                //! Returns true if this iterator points to a valid element of its container.
                bool valid() const
                {
                    return pos_ < chartRef_->size();
                }

            protected:
                
                friend class packed_vector;

                base_iterator(size_type pos, ChartType* chartRef, StorageType* dataRef) :
                    pos_        { pos      },
                    chartRef_   { chartRef },
                    dataRef_    { dataRef  }
                {
                }

            private:
                
                size_type       pos_        = 0;
                ChartType*      chartRef_   = nullptr;
                StorageType*    dataRef_    = nullptr;

        };

    public:
        
        /* --- Type definitions --- */

        typedef base_iterator<Base, chart_type, storage_type>                   iterator;
        typedef base_iterator<const Base, const chart_type, const storage_type> const_iterator;

        /* --- Functions --- */

        packed_vector()
        {
            assert_base_type();
        }
        packed_vector(const packed_vector<Base>& other) :
            chart_  { other.chart_ },
            data_   { other.data_  }
        {
            assert_base_type();
        }
        ~packed_vector()
        {
            for (const auto& entry : chart_)
                destruct(entry);
        }

        /**
        Inserts the specified element into the container.
        \tparam Target Specifies the element data type. This must be from the type 'Base' or a derived type from 'Base'.
        \param[in] val Specifies the new element which is to be added.
        */
        template <typename Target> iterator insert(const_iterator pos, const Target& val)
        {
            insert_element(&val, sizeof(val), pos.pos_, get_base_offset(val));
            return begin() + pos.pos_;
        }

        //! Erases the element at the specified position.
        iterator erase(const_iterator pos)
        {
            erase_element(pos.pos_);
            return begin() + pos.pos_;
        }

        /**
        Appends the specified element to the end of the container.
        \see insert
        */
        template <typename Target> void push_back(const Target& val)
        {
            push_back_element(&val, sizeof(val), get_base_offset(val));
        }

        /**
        Erases the last element from the container. The container must not be empty!
        \see erase
        */
        void pop_back()
        {
            pop_back_element();
        }

        /**
        Returns a reference to the element at the specified position.
        \tparam Target Specifies the output target class. This must be a derived type of 'Base' or the same type.
        \param[in] index Specifies the index of the element which is to be accessed.
        \return Reference to the element.
        \throws std::out_of_range If 'pos' is out of range (see 'std::vector::at').
        \throws std::bad_cast If 'Target' is not the correct class for the accessed element.
        */
        template <typename Target> Target& get(size_type pos)
        {
            assert_target_type<Target>();
            return dynamic_cast<Target&>(*get_base(pos));
        }
        //! Returns a constant reference to the element at the specified index.
        template <typename Target> const Target& get(size_type pos) const
        {
            assert_target_type<Target>();
            return dynamic_cast<const Target&>(*get_base(pos));
        }

        //! Returns a pointer to the element at the specified position.
        template <typename Target> Target* get_ptr(size_type pos)
        {
            assert_target_type<Target>();
            return dynamic_cast<Target*>(get_base(pos));
        }
        //! Returns a constant pointer to the element at the specified position.
        template <typename Target> const Target* get_ptr(size_type pos) const
        {
            assert_target_type<Target>();
            return dynamic_cast<const Target*>(get_base(pos));
        }

        //! Returns the size (or rather number of elements) of this container.
        size_type size() const
        {
            return chart_.size();
        }

        //! Returns true if this container is empty.
        bool empty() const
        {
            return chart_.empty();
        }

        //! Returns an iterator which points to the first element.
        iterator begin()
        {
            return iterator(0, &chart_, &data_);
        }
        //! Returns a constant iterator which points to the first element.
        const_iterator begin() const
        {
            return const_iterator(0, &chart_, &data_);
        }

        //! Returns an iterator which points after the last element.
        iterator end()
        {
            return iterator(size(), &chart_, &data_);
        }
        //! Returns a constant iterator which points after the last element.
        const_iterator end() const
        {
            return const_iterator(size(), &chart_, &data_);
        }

        /* --- Operators --- */

        Base& operator [] (size_type pos)
        {
            return *get_base(pos);
        }
        const Base& operator [] (size_type pos) const
        {
            return *get_base(pos);
        }

    private:
        
        /* --- Functions --- */

        void assert_base_type()
        {
            static_assert(!std::is_pointer<Base>::value, "packed_vector does not allow pointer types");
        }

        template <typename Target> void assert_target_type()
        {
            static_assert(std::is_base_of<Base, Target>::value, "'Target' must be a base of 'Base' in packed_vector");
        }

        Base* get_base(const element& entry)
        {
            return reinterpret_cast<Base*>(&data_[entry.offset + entry.base_offset]);
        }
        const Base* get_base(const element& entry) const
        {
            return reinterpret_cast<const Base*>(&data_[entry.offset + entry.base_offset]);
        }

        Base* get_base(size_type pos)
        {
            return get_base(chart_.at(pos));
        }
        const Base* get_base(size_type pos) const
        {
            return get_base(chart_.at(pos));
        }

        void copy_element(const void* data, const element& entry)
        {
            auto byteAligned = reinterpret_cast<const char*>(data);
            for (size_type i = 0; i < entry.size; ++i)
                data_[entry.offset + i] = byteAligned[i];
        }

        void destruct(const element& entry)
        {
            /*
            Call destructor of base class explicitly.
            Since a base class must have a virtual destructor,
            the correct sequence or destructors will be called here.
            */
            get_base(entry)->~Base();
        }

        void insert_element(const void* data, size_type size, size_type pos, size_type base_offset)
        {
            /* Get insertion position */
            auto insertion = chart_.at(pos);

            /* Insert element into chart and update offsets of successive elements */
            element entry { insertion.offset, size, base_offset };
            chart_.insert(chart_.begin() + pos, entry);
            for (size_type i = pos + 1, n = chart_.size(); i < n; ++i)
                chart_[i].offset += size;

            /* Resize buffer */
            auto byteAligned = reinterpret_cast<const char*>(data);
            auto byteAlignedEnd = byteAligned + size;
            data_.insert(data_.begin() + insertion.offset, byteAligned, byteAlignedEnd);
        }

        void erase_element(size_type pos)
        {
            /* Get entry to erase */
            auto entry = chart_.at(pos);

            /* Destruct element */
            destruct(entry);

            /* Erase element from chart and update offsets of successive elements */
            chart_.erase(chart_.begin() + pos);
            for (size_type i = pos, n = chart_.size(); i < n; ++i)
                chart_[i].offset -= entry.size;

            /* Erase element from data storage */
            data_.erase(data_.begin() + entry.offset, data_.begin() + entry.offset + entry.size);
        }

        void push_back_element(const void* data, size_type size, size_type base_offset)
        {
            /* Add element to chart */
            element entry { data_.size(), size, base_offset };
            chart_.push_back(entry);

            /* Resize buffer and copy element into buffer */
            data_.resize(entry.offset + size);
            copy_element(data, entry);
        }

        void pop_back_element()
        {
            /* Destruct element */
            destruct(chart_.back());

            /* Erase last element from the chart and data storage */
            data_.resize(data_.size() - chart_.back().size);
            chart_.pop_back();
        }

};


} // /namespace ext


#endif


