cmake_minimum_required(VERSION 3.0...3.5)
project(tui)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ncurses package
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Include directories
include_directories(tui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Window.cpp
    src/Border.cpp
    src/Color.cpp
)

# Executable
add_executable(tui ${SOURCES})

# 

# Link against ncurses library
target_link_libraries(tui ${CURSES_LIBRARIES})
