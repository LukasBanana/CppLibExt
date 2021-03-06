Introduction
============

This is a small library of C++11 extensions that are predominantly compatible with the STL.

Featured extensions:

| Feature | Type | Description |
|---------|:----:|-------------|
| `bit_mask` | class | Bit mask/ flags/ options class. |
| `command_line` | class | Command line parser and data model for command line arguments and options. |
| `cstring_view` | class | Alternative to `std::string_view` from C++17, but with null terminated strings. |
| `grid_vector` | wrapper | Simple wrapper of std::vector for 2-dimensional element access. |
| `join_string` | function | Joins a string with fixed and optional values (e.g. for localization). |
| `local_vector` | class | Container that only occupies the stack but with compatible interface to `std::vector`. |
| `member_function` | class | Alternative to `std::function` to get access to the function pointer address. |
| `multi_array` | class | Multi dimensional array, similar to std::array. |
| `path` | class | Path string manager, iterator, and beautifier. |
| `range_iterator` | class | Iterator which keeps track of its range. |

Examples
========
### Example for `ext::multi_array`
```cpp
// Classic C array
int classicArray[4][10][2];
classicArray[2][5][0] = 4;

// boost::multi_array
boost::multi_array<int, 3> boostArray(boost::extents[4][10][2]);
boostArray[2][5][0] = 4;

// ext::multi_array
ext::multi_array<int, 4, 10, 2> multiArray;
multiArray[2] = 3; // Set a value for an entire 'slice' (i.e. multiArray[2][x][y] = 3 for all 0 <= x < 10 and 0 <= y < 2)
multiArray[2][5][0] = 4; // Zero overhead in memory and speed compared to 'classicArray' when compiled in release mode (i.e. optimizations enabled)
```

### Example for `ext::join_string`
```cpp
// output is "undeclared identifier foo_bar"
std::cout << ext::join_string("undeclared identifier {0}", { "foo_bar" }) << std::endl;

// output is "always first, sometimes second"
std::cout << ext::join_string("always {0}[, sometimes {1}]", { "first", "second" }) << std::endl;

// output is "always first"
std::cout << ext::join_string("always {0}[, sometimes {1}]", { "first", "" }) << std::endl;

// output is "always first"
std::cout << ext::join_string("always {0}[, sometimes {1}]", { "first" }) << std::endl;

// output is "one 1, two 2, three 3"
std::cout << ext::join_string("one {0}[, two {1}[, three {2}]]", { "1", "2", "3" }) << std::endl;

// output is "one 1"
std::cout << ext::join_string("one {0}[, two {1}[, three {2}]]", { "1", "", "3" }) << std::endl;

// output is "one 1, three 3"
std::cout << ext::join_string("one {0}[, two {1}][, three {2}]", { "1", "", "3" }) << std::endl;
```

### Example for `ext::member_function`
```cpp
class Widget {
    int x_;
private:
    void set(int x) { x_ = x; }
    int get() const { return x_; }
    void print() { std::cout << x_ << std::endl; }
};

/* ... */

ext::member_function<Widget, void(int)>   setter  = &Widget::set;
ext::member_function<Widget, int() const> getter  = &Widget::get;
ext::member_function<Widget, void()>      printer = &Widget::print;

// Call member functions
Widget w;
setter(w, 42); // alternative to "w.set(42)"
printer(w, getter(w)); // alternative to "w.print(w.get())"

// Print raw function pointer addresses
std::cout << "Widget::set   = " << setter.ptr() << std::endl;
std::cout << "Widget::get   = " << getter.ptr() << std::endl;
std::cout << "Widget::print = " << petter.ptr() << std::endl;
```
