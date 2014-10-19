/*
 * tests.cpp file
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <iostream>
#include <cstdlib>
#include <vector>

#include <cpplibext/multi_array.hpp>
#include <cpplibext/range_iterator.hpp>
#include <cpplibext/make_array.hpp>
#include <cpplibext/packed_vector.hpp>


using namespace ext;

#ifdef _MSC_VER
#   define NOINLINE __declspec(noinline) 
#else
#   define NOINLINE
#endif

typedef int ClassicArray[10][10][10];
typedef multi_array<int, 10, 10, 10> LukasArray;

NOINLINE int Get(const ClassicArray& a, size_t x, size_t y, size_t z)
{
    return a[x][y][z];
}

NOINLINE int Get(const LukasArray& a, size_t x, size_t y, size_t z)
{
    return a[x][y][z];
}

void packed_vector_test()
{
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
        int z = 3;
    };
    
    packed_vector<A> list;

    A a;
    B b;
    b.x = 4;

    auto aRef = &a;
    auto bRef = &b;
    A* bARef = &b;

    list.push_back(a);
    list.push_back(b);

    auto aGet = list.get<A>(0);
    auto bGet = list.get<B>(1);

    b.x = -3;
    b.y = 7;
    list.insert(list.begin(), b);

    for (size_t i = 0; i < list.size(); ++i)
    {
        auto a = list.get_ptr<A>(i);

        int _unused=0;
    }

    for (auto it = list.begin(); it != list.end(); ++it)
    {
        auto obj = *it;
        int _unused=0;
    }

    for (auto& entry : list)
    {
        auto obj = dynamic_cast<B*>(&entry);
        if (obj)
            int _unused=0;
    }

    auto it = list.begin();
    auto obj0 = it[0];
}

void packed_vector_test2()
{
    struct BaseA
    {
        virtual ~BaseA() {};
    
        int ba_x;
    };

    struct BaseB
    {
        virtual ~BaseB() {};
    
        int bb_x;
    };

    struct DerivedB : public BaseA, public BaseB
    {
        int db_x;
    };

    packed_vector<BaseA> list;
    list.push_back(DerivedB());
    
    DerivedB derived;
    BaseA* ptr = &derived;

    DerivedB* ptr2 = dynamic_cast<DerivedB*>(ptr);
    if (!ptr2)
        std::cerr << "Error: ptr2 == nulltpr\n";
    else if (ptr2 != &derived)
        std::cerr << "Error: ptr wrong value\n";
    
    DerivedB* ptr3 = list.get_ptr<DerivedB>(0);
    if (!ptr3)
        std::cerr << "Error: ptr3 == nulltpr\n";
}

int main()
{
    // C array comparision
    ClassicArray A;
    LukasArray B;

    A[1][2][3] = 5;
    B[0][2][2] = 6;

    for (size_t x = 0; x < 2; ++x)
    {
        std::cout << Get(A, x, x, x) << " ";
        std::cout << Get(B, x, x, x) << " ";
    }

    // Some array tests
    typedef multi_array<int, 3, 4, 2> my_array_t;

    my_array_t my_array;
    std::cout << "# of dimensions:\t" << my_array_t::num_dimensions << std::endl;
    std::cout << "# of elements:\t\t" << my_array_t::num_elements << std::endl;
    std::cout << "size in bytes:\t\t" << my_array_t::data_size << std::endl;

    std::cout << std::endl;

    try
    {
        /* --- multi_array tests --- */

        for (size_t x = 0; x < 3; ++x)
        {
            for (size_t y = 0; y < 4; ++y)
            {
                for (size_t z = 0; z < 2; ++z)
                {
                    auto x_ = x + 1;
                    auto y_ = y + 1;
                    auto z_ = z + 1;

                    my_array.at(x).at(y).at(z) = x_*y_*z_;
                }
            }
        }

        my_array[1] = -1;
        my_array[2][1] = -3;
        my_array[2][2][0] = -5;
        
        for (size_t x = 0; x < 3; ++x)
        {
            for (size_t y = 0; y < 4; ++y)
            {
                for (size_t z = 0; z < 2; ++z)
                {
                    std::cout << "my_array[" << x << "]" << "[" << y << "]" << "[" << z << "] = " << my_array[x][y][z] << std::endl;
                }
            }
        }
        
        std::cout << std::endl;

        multi_array<int, 3, 2> my_simple_array;
        my_simple_array.fill(4);

        multi_array<int, 5> single_dim_array { 1, 2, 3, 4, 5 };

        for (auto& v : my_simple_array)
        {
            v = std::rand() % 10;
            std::cout << "single_dim_array[x] (:= random) = " << v << std::endl;
        }
        std::cout << std::endl;

        std::cout << "my_simple_array sorted:" << std::endl;
        std::sort(my_simple_array.begin(), my_simple_array.end());

        for (auto& v : my_simple_array)
            std::cout << "single_dim_array[x] = " << v << std::endl;
        std::cout << std::endl;

        std::cout << "single_dim_array:" << std::endl;
        for (auto it = single_dim_array.begin(); it != single_dim_array.end(); ++it)
            std::cout << "single_dim_array[x] = " << *it << std::endl;
        std::cout << std::endl;

        std::cout << "single_dim_array reverse:" << std::endl;
        for (auto it = single_dim_array.rbegin(); it != single_dim_array.rend(); ++it)
            std::cout << "single_dim_array[x] = " << *it << std::endl;
        std::cout << std::endl;

        my_simple_array.at(2) = 0;

        std::cout << "my_simple_array.empty() = " << my_simple_array.empty() << std::endl;

        std::cout << std::endl;
        std::cout << "my_array.slices<0>() = " << my_array.slices<0>() << std::endl;
        std::cout << "my_array.slices<1>() = " << my_array.slices<1>() << std::endl;
        std::cout << "my_array.slices<2>() = " << my_array.slices<2>() << std::endl;

        std::cout << std::endl;
        std::cout << "my_array.slices(0) = " << my_array.slices(0) << std::endl;
        std::cout << "my_array.slices(1) = " << my_array.slices(1) << std::endl;
        std::cout << "my_array.slices(2) = " << my_array.slices(2) << std::endl;

        /* --- range_iterator tests --- */

        #ifdef _MSC_VER
        
        std::cout << std::endl;
        //for (const_range_iterator<decltype(my_array)> it { my_array }; !it.reached_end(); ++it)
        for (const_range_iterator<my_array_t> it { my_array }; !it.reached_end(); ++it)
            std::cout << "range_iterator: " << *it << std::endl;

        struct TestStruct
        {
            int a, b;
        };

        std::vector<TestStruct> testVec { { 1, 2 }, { 3, 4 } };

        for (range_forward_iterator<decltype(testVec)> it { testVec }; !it.reached_end(); ++it)
            it->a = 42;

        std::cout << std::endl;
        for (auto s : testVec)
            std::cout << "a = " << s.a << ", b = " << s.b << std::endl;

        #endif
        
        /* --- packed_vector tests --- */

        packed_vector_test();
        packed_vector_test2();
    }
    catch (const std::exception& err)
    {
        std::cout << "EXCEPTION: \"" << err.what() << "\"" << std::endl;
    }

    auto ptr1 = make_shared_array<int>(10);
    //auto ptr2 = std::make_unique<int[]>(10);

    std::cout << std::endl;
    system("pause");
    
    return 0;
}
