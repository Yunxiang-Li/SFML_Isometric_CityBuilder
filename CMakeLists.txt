# This CMakeLists.txt works for CLion under Windows 10 OS.

# Set up the minimum version of cmake
cmake_minimum_required(VERSION 3.19)

# Set the project name
project(SFML_CityBuilder)

# Link SFML libraries statically
set(SFML_STATIC_LIBRARIES TRUE)

# Set the C++ standard to be used
set(CMAKE_CXX_STANDARD 14)

# Set cmake warning.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Set the path to SFML library
set(SFML_DIR "D:/SFML-2.5.1/lib/cmake/SFML")

# Include header files folder
include_directories(include/)

# look for SFML package and add it
find_package(SFML REQUIRED COMPONENTS graphics window system)

# Set output directory to the bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add the executable of the program
add_executable(SFML_CityBuilder src/main.cpp src/Game.cpp src/MainGameState.cpp src/MainMenuGameState.cpp)

# Set linked libraries
target_link_libraries(SFML_CityBuilder PUBLIC sfml-system sfml-window sfml-graphics)