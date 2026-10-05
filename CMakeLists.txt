cmake_minimum_required(VERSION 3.22)
project(torrentd CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Suppress CMP0167 warning regarding FindBoost removal in modern CMake
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif()

# Optimization flags for ARM64 / Tensor G4
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -DNDEBUG")

include(FetchContent)

# 1. Fetch Boost via source archive
FetchContent_Declare(
    boost
    URL https://archives.boost.io/release/1.92.0/source/boost_1_92_0.tar.gz
)
set(BOOST_INCLUDE_LIBRARIES system)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_MakeAvailable(boost)

set(Boost_INCLUDE_DIR "${boost_SOURCE_DIR}" CACHE PATH "Boost include directory" FORCE)
set(Boost_FOUND TRUE CACHE BOOL "" FORCE)

# 2. Configure libtorrent to use built-in crypto and disable WebTorrent
set(crypto "built-in" CACHE STRING "" FORCE)
set(webtorrent OFF CACHE BOOL "" FORCE)

# 3. Fetch libtorrent-rasterbar (v2.1.2)
FetchContent_Declare(
    libtorrent
    GIT_REPOSITORY https://github.com/arvidn/libtorrent.git
    GIT_TAG        v2.1.2
)
set(build_examples OFF)
set(build_tests OFF)
FetchContent_MakeAvailable(libtorrent)

# 4. Your core executable binary
add_executable(torrentd
    src/main.cpp
)

target_link_libraries(torrentd
    PRIVATE
    libtorrent
)
