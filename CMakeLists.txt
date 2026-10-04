cmake_minimum_required(VERSION 3.22)
project(torrentd CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization flags for ARM64 / Tensor G4
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -DNDEBUG")

include(FetchContent)

# 1. Fetch Boost
FetchContent_Declare(
    boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG        boost-1.92.0
)
set(BOOST_INCLUDE_LIBRARIES system)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_MakeAvailable(boost)

# 2. Disable WebTorrent to bypass libdatachannel and OpenSSL requirements
set(webtorrent OFF CACHE BOOL "" FORCE)

# 3. Fetch libtorrent-rasterbar
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
