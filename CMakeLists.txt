cmake_minimum_required(VERSION 3.20)
project(GFML VERSION 1.0.0 LANGUAGES CXX)
# ── Clone and build ──────────────────────────────────────────────────────────
# cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake
# cmake --build build

# ── Install system-wide (then usable via find_package) ───────────────────────
# cmake --install build --prefix /usr/local

# ── C++ Standard ─────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Options ───────────────────────────────────────────────────────────────────
option(GFML_BUILD_EXAMPLES "Build examples" ON)
option(GFML_BUILD_SHARED   "Build as shared library" OFF)

# ── SFML ──────────────────────────────────────────────────────────────────────
# ── SFML 2.6 — find locally or fetch exact version ───────────────────────────
set(SFML_DIR "/opt/sfml2/lib/cmake/SFML/")

if(NOT DEFINED SFML_DIR AND DEFINED ENV{SFML_DIR})
    set(SFML_DIR $ENV{SFML_DIR})
endif()

find_package(SFML 2.6 COMPONENTS graphics window system audio QUIET)

if(NOT SFML_FOUND)
    message(STATUS "SFML 2.6 not found locally — fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG        2.6.2          # pinned, not a moving branch
            GIT_SHALLOW    TRUE
    )
    set(SFML_BUILD_AUDIO      ON  CACHE BOOL "" FORCE)
    set(SFML_BUILD_GRAPHICS   ON  CACHE BOOL "" FORCE)
    set(SFML_BUILD_WINDOW     ON  CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK    OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC        OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_EXAMPLES   OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SFML)
    message(STATUS "SFML 2.6.2 fetched successfully")
endif()

# ── Library target ────────────────────────────────────────────────────────────
file(GLOB_RECURSE GFML_SOURCES "src/*.cpp")

if(GFML_BUILD_SHARED)
    add_library(GFML SHARED ${GFML_SOURCES})
else()
    add_library(GFML STATIC ${GFML_SOURCES})
endif()

target_include_directories(GFML
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(GFML
        PUBLIC sfml-graphics sfml-window sfml-system sfml-audio
)

target_compile_definitions(GFML PUBLIC
        GFML_VERSION_MAJOR=1
        GFML_VERSION_MINOR=0
        GFML_VERSION_PATCH=0
)

# ── Examples ──────────────────────────────────────────────────────────────────
if(GFML_BUILD_EXAMPLES)
    add_executable(GFML_Example examples/main.cpp)
    target_link_libraries(GFML_Example PRIVATE GFML)
endif()

# ── Install ───────────────────────────────────────────────────────────────────
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS GFML
        EXPORT GFMLTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/GFML
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT GFMLTargets
        FILE      GFMLTargets.cmake
        NAMESPACE GFML::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GFML
)

configure_package_config_file(
        cmake/GFMLConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/GFMLConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GFML
)

write_basic_package_version_file(
        GFMLConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/GFMLConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/GFMLConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GFML
)

# ── Windows DLL handling ──────────────────────────────────────────────────────
if(WIN32)
    # After build, copy all SFML DLLs next to the executable automatically
    add_custom_command(TARGET GFML_Example POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:GFML_Example>
            $<TARGET_FILE_DIR:GFML_Example>
            COMMAND_EXPAND_LISTS
    )
endif()