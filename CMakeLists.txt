cmake_minimum_required(VERSION 3.20)

project(PartBuddy
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Locate SQLite
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED IMPORTED_TARGET sqlite3)

add_executable(PartBuddy
    src/main.cpp
    src/Inventory.cpp
    src/Database.cpp
)

# Project headers.
target_include_directories(PartBuddy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# SQLite.
target_link_libraries(PartBuddy PRIVATE
    PkgConfig::SQLITE3
)

# Compiler warnings.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(PartBuddy PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()