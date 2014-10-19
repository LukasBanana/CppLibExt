/*
 * packet_vector.hpp file
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef _CPPLIBEXT_PACKED_VECTOR_H_
#define _CPPLIBEXT_PACKED_VECTOR_H_


#include <vector>
#include <type_traits>
#include <algorithm>
#include <exception>


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
*/
template <typename Base> class packed_vector
{
    
    public:
        
        /* --- Type definitions --- */

        typedef std::vector<char>       storage_type;
        typedef storage_type::size_type size_type;

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

        template <typename Target> void push_back(const Target& val)
        {
            assert_target_type<Target>();
            push_back_raw(&val, sizeof(val));
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

    private:
        
        struct element
        {
            size_type offset, size;
        };

        void assert_base_type()
        {
            static_assert(!std::is_pointer<Base>::value, "packed_vector does not allow pointer types");
        }

        template <typename Target> void assert_target_type()
        {
            static_assert(std::is_base_of<Base, Target>::value, "'Target' must be a base of 'Base' in packed_vector");
        }

        void copy(const void* data, const element& entry)
        {
            auto byteAligned = reinterpret_cast<const char*>(data);
            for (size_type i = 0; i < entry.size; ++i)
                data_[entry.offset + i] = byteAligned[i];
        }

        void push_back_raw(const void* data, size_type size)
        {
            /* Add element to chart */
            element entry { data_.size(), size };
            chart_.push_back(entry);

            /* Resize buffer */
            data_.resize(entry.offset + size);

            /* Copy element into buffer */
            copy(data, entry);
        }

        Base* get_base(size_type pos)
        {
            auto entry = chart_.at(pos);
            return reinterpret_cast<Base*>(&data_[entry.offset]);
        }
        const Base* get_base(size_type pos) const
        {
            auto entry = chart_.at(pos);
            return reinterpret_cast<const Base*>(&data_[entry.offset]);
        }

        std::vector<element> chart_;
        storage_type data_;

};


} // /namespace ext


#endif


