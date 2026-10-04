cmake_minimum_required(VERSION 3.22)
project(streamer_engine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization flags for ARM64 / Tensor G4
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -DNDEBUG")

# Include directories (assuming Boost and libtorrent are fetched or included via FetchContent)
# For a production repo, you can use FetchContent or submodule them.
find_package(Boost REQUIRED COMPONENTS system)

add_executable(streamer_engine
    src/main.cpp
)

target_link_libraries(streamer_engine
    PRIVATE
    Boost::system
    # libtorrent target goes here
)
