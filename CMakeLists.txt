cmake_minimum_required(VERSION 3.20)


project(TuiTetris LANGUAGES CXX)

file(GLOB_RECURSE TETRIS_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(tetris ${TETRIS_SOURCES})

target_include_directories(tetris PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_features(tetris PRIVATE cxx_std_20)

target_compile_options(tetris PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
