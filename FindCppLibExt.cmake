
# Custom CMake module for finding "CppLibExt" include directory
# Written by Lukas Hermanns on 24/09/2014

# Find library

find_path(
	CppLibExt_INCLUDE_DIR
	NAMES
		cpplibext/multi_array.hpp
	HINTS
		${CppLibExt_ROOT_DIR}/cpplibext/multi_array.hpp
)

# Setup package handle

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	CppLibExt
	DEFAULT_MSG
	CppLibExt_INCLUDE_DIR
)

if(CPPLIBEXT_FOUND)
	set(CppLibExt_FOUND TRUE)
endif(CPPLIBEXT_FOUND)
