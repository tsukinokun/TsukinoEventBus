cmake_minimum_required(VERSION 3.12)
project(TsukinoEventBus LANGUAGES CXX)

# C++17 を必須にする
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ライブラリ（ヘッダオンリーなので INTERFACE）
add_library(TsukinoEventBus INTERFACE)
target_include_directories(TsukinoEventBus INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Examples をまとめて追加
add_executable(example1_basic examples/example1_basic.cpp)
target_link_libraries(example1_basic TsukinoEventBus)

add_executable(example2_priority examples/example2_priority.cpp)
target_link_libraries(example2_priority TsukinoEventBus)

add_executable(example3_inheritance examples/example3_inheritance.cpp)
target_link_libraries(example3_inheritance TsukinoEventBus)

add_executable(example4_update_callback examples/example4_update_callback.cpp)
target_link_libraries(example4_update_callback TsukinoEventBus)

add_executable(example5_raii examples/example5_raii.cpp)
target_link_libraries(example5_raii TsukinoEventBus)
