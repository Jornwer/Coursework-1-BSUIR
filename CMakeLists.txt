cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

project(Coursework)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(GCC_COVERAGE_COMPILE_FLAGS "-O3 -static -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wredundant-decls -Wshadow -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(Coursework
        lib/SHA256.cpp
        lib/SHA256.h
        lib/json.hpp
        src/Admin.cpp
        src/Functions.cpp
        src/Header.h
        src/User.cpp
        src/Users/AbstractUser.cpp
        src/Users/AbstractUser.h
        src/Users/User.cpp
        src/Users/User.h
        src/Users/Admin.cpp
        src/Users/Admin.h
        src/Data/Date.cpp
        src/Data/Date.h
        src/Users/AbstractUserImpl.h)