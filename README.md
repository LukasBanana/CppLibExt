Introduction
============

This is a small library of C++11 extensions.

Current features:
- multi_array

Examples
========
```cpp
/* --- multi_array --- */

// Classic C array:
int classicArray[4][10][2];
classicArray[2][5][0] = 4;

// boost::multi_array
boost::multi_array<int, 3> boostArray(boost::extents[4][10][2]);
boostArray[2][5][0] = 4;

// multi_array:
multi_array<int, 4, 10, 2> multiArray;
multiArray[2] = 3; // Set a value for an entire 'slice' (i.e. multiArray[2][x][y] = 3 for all 0 <= x < 10 and 0 <= y < 2)
multiArray[2][5][0] = 4; // No overhead here in memory and speed compared to 'classicArray'
```
