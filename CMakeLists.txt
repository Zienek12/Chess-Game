cmake_minimum_required(VERSION 3.10)
project(Chess LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
 if (MSVC)
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE)
        string (REGEX REPLACE "/W[0-4]" "" ${flag_var} "${${flag_var}}")
    endforeach()
    add_compile_options(/W4)
endif()

else()
    add_compile_options(-O2)
endif()


add_subdirectory(Chess)
