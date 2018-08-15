/*
 * tests.cpp file
 * 
 * Copyright (C) 2014-2018 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <memory>

#include <cpplibext/multi_array.hpp>
#include <cpplibext/range_iterator.hpp>
#include <cpplibext/make_shared_array.hpp>
#include <cpplibext/make_unique.hpp>
#include <cpplibext/grid_vector.hpp>
#include <cpplibext/command_line.hpp>
#include <cpplibext/bit_mask.hpp>
#include <cpplibext/join_string.hpp>
#include <cpplibext/path.hpp>
#include <cpplibext/fixed_uint.hpp>
#include <cpplibext/cstring_view.hpp>
#include <cpplibext/generic_string.hpp>
#include <cpplibext/growing_stack.hpp>


using namespace ext;

#ifdef _MSC_VER
#   define NOINLINE __declspec(noinline) 
#else
#   define NOINLINE
#endif

static void test_headline(const std::string& s)
{
    std::cout << std::endl << std::endl << s << std::endl << std::string(s.size(), '-') << std::endl << std::endl;
}

#define TEST_HEADLINE test_headline(__FUNCTION__)

/* --- multi_array tests --- */

typedef int ClassicArray[10][10][10];
typedef multi_array<int, 10, 10, 10> MyArrayType;

NOINLINE int Get(const ClassicArray& a, size_t x, size_t y, size_t z)
{
    return a[x][y][z];
}

NOINLINE int Get(const MyArrayType& a, size_t x, size_t y, size_t z)
{
    return a[x][y][z];
}

static void multi_array_test()
{
    TEST_HEADLINE;

    // C array comparision
    ClassicArray A;
    MyArrayType B;

    #if 1 //NOTE: disable to check how uninitialized ClassicArray and MyArrayType compare
    memset(A, 0, sizeof(A));
    memset(&B, 0, sizeof(B));
    #endif

    A[1][2][3] = 5;
    B[0][2][2] = 6;

    for (size_t x = 0; x < 2; ++x)
    {
        std::cout << Get(A, x, x, x) << " ";
        std::cout << Get(B, x, x, x) << " ";
    }

    // Some array tests
    typedef multi_array<int, 3, 4, 2> my_array_t;

    std::cout << std::endl;

    my_array_t my_array;
    std::cout << "# of dimensions:\t" << my_array_t::num_dimensions << std::endl;
    std::cout << "# of elements:\t\t" << my_array_t::num_elements << std::endl;
    std::cout << "size in bytes:\t\t" << my_array_t::data_size << std::endl;

    std::cout << std::endl;

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
}

/* --- grid_vector test -- */

static void grid_vector_test()
{
    TEST_HEADLINE;

    grid_vector<int> grid;
    grid.resize(10, 10);

    grid(0, 0) = 5;

    std::cout << "grid size = ( " << grid.width() << ", " << grid.height() << " )" << std::endl;
}

/* --- command_line test --- */

static void command_line_test(int argc, char* argv[])
{
    TEST_HEADLINE;

    command_line_parser parser;
    auto cmdLine = parser.parse(argc - 1, argv + 1);

    std::cout << "command line:" << std::endl;
    for (const auto& arg : cmdLine.arguments())
    {
        std::cout << "  " << arg.value() << std::endl;
        for (auto opt : arg.options())
            std::cout << "    " << opt << std::endl;
    }
}

/* --- bit_mask test --- */

static void bit_mask_test()
{
    TEST_HEADLINE;

    bit_mask<int> flags;

    flags << 0x02 << 0x10 << 0x08 << 0x200;
    flags >> 0x10;

    std::cout << "flags capacity: " << flags.capacity() << std::endl;
    std::cout << "flags size: " << flags.size() << std::endl;

    for (auto f : flags)
        std::cout << "flag set: " << std::hex << f << std::endl;

    int x=0;
}

/* --- join_string test --- */

static void join_string_test()
{
    TEST_HEADLINE;

    std::cout << join_string("undeclared identifier {0}", { "foo_bar" }) << std::endl;
    std::cout << join_string("always {0}[, sometimes {1}]", { "first", "second" }) << std::endl;
    std::cout << join_string("always {0}[, sometimes {1}]", { "first", "" }) << std::endl;
    std::cout << join_string("always {0}[, sometimes {1}]", { "first" }) << std::endl;
    std::cout << join_string("one {0}[, two {1}[, three {2}]]", { "1", "2", "3" }) << std::endl;
    std::cout << join_string("one {0}[, two {1}[, three {2}]]", { "1", "", "3" }) << std::endl;
    std::cout << join_string("one {0}[, two {1}][, three {2}]", { "1", "", "3" }) << std::endl;
    std::cout << std::endl;

    std::wcout << join_string(L"undeclared identifier {0}", { L"foo_bar" }) << std::endl;
    std::wcout << join_string(L"always {0}[, sometimes {1}]", { L"first", L"second" }) << std::endl;
    std::wcout << join_string(L"always {0}[, sometimes {1}]", { L"first", L"" }) << std::endl;
    std::wcout << join_string(L"always {0}[, sometimes {1}]", { L"first" }) << std::endl;
    std::wcout << join_string(L"one {0}[, two {1}[, three {2}]]", { L"1", L"2", L"3" }) << std::endl;
    std::wcout << join_string(L"one {0}[, two {1}[, three {2}]]", { L"1", L"", L"3" }) << std::endl;
    std::wcout << join_string(L"one {0}[, two {1}][, three {2}]", { L"1", L"", L"3" }) << std::endl;
    std::cout << std::endl;

    std::cout << join_string("TEST: \\\\ [\\[\\{{0}\\}\\]]", { "test" }) << std::endl;
    std::cout << std::endl;

    const std::string s0 = "foo_bar";
    const std::string js0 = "undeclared identifier {0}";

    std::cout << join_string(js0, { s0 }) << std::endl;
    std::cout << std::endl;

    std::vector<std::string> v0 { "test1", "test2" };
    std::cout << join_string("TEST1: {0}[, TEST2: {1}]", std::begin(v0), std::end(v0)) << std::endl;

    std::vector<ext::cstring_view> v1 { "test1", "test2" };
    std::cout << join_string("TEST1: {0}[, TEST2: {1}]", std::begin(v1), std::end(v1)) << std::endl;
}

/* --- path test --- */

static void path_test()
{
    TEST_HEADLINE;

    #if 0
    path path0 = "/Foo/Bar";
    #else
    path path0 = "Foo/Bar";
    #endif
    path path1 = "../Lol";

    auto path2 = path0 + path1;

    path path3 = "Hello/World/Foo/";
    path path4 = "../../Bar";
    path path5 = path3 + path4;

    std::cout << "path0 = \"" << path0 << '\"' << std::endl;
    std::cout << "path1 = \"" << path1 << '\"' << std::endl;
    std::cout << "path2 = path0 + path1 = \"" << path2 << '\"' << std::endl;
    std::cout << "path2.size() = " << path2.size() << std::endl;
    std::cout << "path2.root() = " << std::boolalpha << path2.root() << std::endl;
    std::cout << "path2.absolute() = " << std::boolalpha << path2.absolute() << std::endl;
    std::cout << "path3 = \"" << path3 << '\"' << std::endl;
    std::cout << "path4 = \"" << path4 << '\"' << std::endl;
    std::cout << "path5 = path3 + path4 = \"" << path5 << '\"' << std::endl;

    std::cout << "sub paths of path2:" << std::endl;
    for (const auto& sub : path2)
        std::cout << " -> \"" << sub << '\"' << std::endl;

    std::cout << "sub paths of path2 reversed:" << std::endl;

    auto it = path2.end();

    do
    {
        --it;
        std::cout << " -> \"" << *it << '\"' << std::endl;
    }
    while (it != path2.begin());

    {
        std::cout << "path::erase" << std::endl;

        auto p = path2;

        for (int i = 0; !p.empty(); ++i)
        {
            std::cout << "  t" << i << ": " << p << std::endl;
            p.erase(p.begin());
        }
    }

    {
        std::cout << "path::erase" << std::endl;

        auto p = path2;

        std::cout << "  t0: " << p << std::endl;
        p.erase(++p.begin());
        std::cout << "  t1: " << p << std::endl;
        p.erase(++p.begin());
        std::cout << "  t2: " << p << std::endl;
    }

    {
        std::cout << "path::insert" << std::endl;

        auto p = path2;

        std::cout << "  t0: " << p << std::endl;
        p.insert(++p.begin(), path2);
        std::cout << "  t1: " << p << std::endl;
        p.insert(++p.begin(), "../../");
        std::cout << "  t2: " << p << std::endl;
    }
}

/* --- fixed_uint test --- */

static void fixed_uint_test()
{
    TEST_HEADLINE;

    fixed_uint256 a, b;

    a = 42u;
    b = 17u;

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;




}

/* --- cstring_view --- */

static void cstring_view_test()
{
    TEST_HEADLINE;

    std::string s1 = "Foo bar";

    s1.compare(1, 2, "foo", 3, 4);

    cstring_view sv1 = "Hello World";
    cstring_view sv2 = s1;

    std::cout << "sv1 = " << sv1 << std::endl;
    std::cout << "sv2 = " << sv2 << std::endl;

    std::cout << "sv1 == sv1 => " << std::boolalpha << (sv1 == sv1) << std::endl;
    std::cout << "sv1 != sv1 => " << std::boolalpha << (sv1 != sv1) << std::endl;
    std::cout << "sv1 <  sv1 => " << std::boolalpha << (sv1 <  sv1) << std::endl;
    std::cout << "sv1 <= sv1 => " << std::boolalpha << (sv1 <= sv1) << std::endl;
    std::cout << "sv1 >  sv1 => " << std::boolalpha << (sv1 >  sv1) << std::endl;
    std::cout << "sv1 >= sv1 => " << std::boolalpha << (sv1 >= sv1) << std::endl;

    std::cout << "sv1 == sv2 => " << std::boolalpha << (sv1 == sv2) << std::endl;
    std::cout << "sv1 != sv2 => " << std::boolalpha << (sv1 != sv2) << std::endl;
    std::cout << "sv1 <  sv2 => " << std::boolalpha << (sv1 <  sv2) << std::endl;
    std::cout << "sv1 <= sv2 => " << std::boolalpha << (sv1 <= sv2) << std::endl;
    std::cout << "sv1 >  sv2 => " << std::boolalpha << (sv1 >  sv2) << std::endl;
    std::cout << "sv1 >= sv2 => " << std::boolalpha << (sv1 >= sv2) << std::endl;

    for (auto c : sv1)
        std::cout << c;
    std::cout << std::endl;

    for (auto it = sv1.rbegin(); it != sv1.rend(); ++it)
        std::cout << *it;
    std::cout << std::endl;
}

/* --- generic_string --- */

static void generic_string_test()
{
    TEST_HEADLINE;

    auto print_gstring = [](const gstring& s)
    {
        std::cout << "gstring(mutable: " << s.is_mutable() << ") = " << s << std::endl;
    };

    auto modify_and_print_gstring = [](gstring& s)
    {
        s.append(", Appendix");
        std::cout << "gstring(mutable: " << s.is_mutable() << ") = " << s << std::endl;
    };

    gstring s1 = "Hello World";
    gstring s2 = std::string("Foo");
    gstring s3 = s1;
    gstring s4 = "This is an ";
    s4.append("Appendix");
    gstring s5;

    std::cout << std::boolalpha;
    print_gstring(s1);
    modify_and_print_gstring(s1);
    std::cout << "s2(mutable: " << s2.is_mutable() << ") = " << s2 << std::endl;
    std::cout << "s3(mutable: " << s3.is_mutable() << ") = " << s3 << std::endl;
    std::cout << "s4(mutable: " << s4.is_mutable() << ") = " << s4 << std::endl;
    std::cout << "s5(mutable: " << s5.is_mutable() << ") = " << s5 << std::endl;
}

/* --- growing_stack --- */

static void growing_stack_test()
{
    TEST_HEADLINE;

    growing_stack<int> myStack;

    myStack.reserve(5);

    myStack.push(1);
    myStack.push(2);
    myStack.push(3);
    myStack.pop();
    myStack.push(4);

    while (!myStack.empty())
    {
        std::cout << "top value: " << myStack.top() << ", capacity: " << myStack.capacity() << std::endl;
        myStack.pop();
    }
}

/* --- main --- */

int main(int argc, char* argv[])
{
    try
    {
        //multi_array_test();

        //grid_vector_test();

        //command_line_test(argc, argv);

        //bit_mask_test();

        //join_string_test();

        //path_test();

        //fixed_uint_test();

        //cstring_view_test();

        //generic_string_test();

        growing_stack_test();
    }
    catch (const std::exception& err)
    {
        std::cout << "EXCEPTION:" << std::endl << err.what() << std::endl;
    }

    auto ptr1 = make_shared_array<int>(10);
    auto ptr2 = make_unique<int>(10);

    std::cout << std::endl;

    #ifdef _WIN32
    system("pause");
    #endif

    return 0;
}
