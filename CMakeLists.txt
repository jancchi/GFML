cmake_minimum_required(VERSION 3.20)
project(GFML VERSION 1.0.0 LANGUAGES CXX)

# 1. Standard C++ Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML 2.6.2
# Note: If SFML is not in a standard location, set(SFML_DIR "path/to/SFML/lib/cmake/SFML")
set(SFML_DIR "/opt/sfml2/lib/cmake/SFML/")

find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)
# Collect all .cpp files in the src directory
file(GLOB_RECURSE GFML_SOURCES "src/*.cpp")

add_library(GFML STATIC ${GFML_SOURCES}
        include/GFML/Button.hpp
        src/Button.cpp
        src/TextArea.cpp)

# PUBLIC ensures that anyone linking to GFML also gets the include path
target_include_directories(GFML PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# GFML depends on SFML
target_link_libraries(GFML PUBLIC sfml-graphics sfml-window sfml-system)

# Uncomment the lines below to build a "sandbox" or "test" app automatically
add_executable(GFML_Example examples/main.cpp
        examples/main.cpp
        examples/main.cpp)
target_link_libraries(GFML_Example PRIVATE GFML)
