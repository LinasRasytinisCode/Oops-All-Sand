cmake_minimum_required(VERSION 3.10)
project(MyRaylibGame)

# This is the magic line that fixes autocomplete in most IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Raylib (Assumes it is installed on your system)
find_package(raylib REQUIRED)

# Add your executable
add_executable(${PROJECT_NAME} main.cpp)

# Link Raylib to your executable
target_link_libraries(${PROJECT_NAME} raylib)