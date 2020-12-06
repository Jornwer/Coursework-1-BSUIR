cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

project(Coursework)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(GCC_COVERAGE_COMPILE_FLAGS "-O3 -static -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wredundant-decls -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(FORT_ENABLE_TESTING OFF CACHE INTERNAL "")

add_executable(Coursework
        lib/SHA256.cpp
        lib/SHA256.h
        lib/json.hpp
        src/Functions.cpp
        src/Header.h
        src/Users/AbstractUser.cpp
        src/Users/AbstractUser.h
        src/Users/User.cpp
        src/Users/User.h
        src/Users/Admin.cpp
        src/Users/Admin.h
        src/Data/Date.cpp
        src/Data/Date.h
        src/Users/AbstractUserImpl.h
        src/Data/Deal.cpp
        src/Data/Deal.h
        src/Data/Catalog.cpp
        src/Data/Catalog.h)

add_subdirectory(lib/libfort)
target_link_libraries(Coursework PRIVATE fort)