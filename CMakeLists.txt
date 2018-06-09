
# === CMake lists for the CppLibExt Project - (04/07/2014) ===

cmake_minimum_required(VERSION 2.8)
project(CppLibExt)


# === Build path ===

set(dir ${CMAKE_CURRENT_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)


# === Global files ===

file(GLOB FilesInclude "${PROJECT_SOURCE_DIR}/include/cpplibext/*.*")
file(GLOB FilesIncludeDetails "${PROJECT_SOURCE_DIR}/include/cpplibext/details/*.*")
file(GLOB FilesTests "${PROJECT_SOURCE_DIR}/tests/*.*")

set(
    LibFiles
    ${FilesInclude}
    ${FilesIncludeDetails}
)


# === Include directories ===

include_directories("${PROJECT_SOURCE_DIR}/include")


# === Source Groups ===

source_group("cpplibext" FILES ${FilesInclude})
source_group("cpplibext\\details" FILES ${FilesIncludeDetails})
source_group("tests" FILES ${FilesTests})


# === Executable ===

add_library(CppLibExt STATIC ${LibFiles})

add_executable(CppLibTests ${FilesTests})

set_target_properties(CppLibExt PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(CppLibTests PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features(CppLibExt PRIVATE cxx_variadic_templates)
target_compile_features(CppLibTests PRIVATE cxx_variadic_templates)

