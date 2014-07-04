
# === CMake lists for the CppLibExt Project - (04/07/2014) ===

cmake_minimum_required(VERSION 2.8)
project(CppLibExt)


# === Build path ===

set(dir ${CMAKE_CURRENT_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)


# === Global files ===

file(GLOB_RECURSE FilesInclude ${PROJECT_SOURCE_DIR}/include/*.*)
file(GLOB_RECURSE FilesTests ${PROJECT_SOURCE_DIR}/tests/*.*)

# === Include directories ===

include_directories("${PROJECT_SOURCE_DIR}/include")


# === Executable ===

add_library(CppLibExt STATIC ${FilesInclude})

add_executable(CppLibTests ${FilesTests})

set_target_properties(CppLibExt PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(CppLibTests PROPERTIES LINKER_LANGUAGE CXX)


