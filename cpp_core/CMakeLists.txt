cmake_minimum_required(VERSION 3.20)
project(matrixlib_practice VERSION 1.0.0 LANGUAGES CXX)

option(MATRIXLIB_BUILD_TESTS "Build unit tests" ON)
option(MATRIXLIB_ENABLE_SANITIZERS "Enable ASan/UBSan for debug builds" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sensible defaults
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_library(matrixlib
    matrix/src/matrix.cpp
    matrix/src/mul_naive.cpp
    matrix/src/mul_simd_avx.cpp
    matrix/src/det.cpp
    matrix/src/lu.cpp
)
target_include_directories(matrixlib PUBLIC matrix/include)
target_compile_features(matrixlib PUBLIC cxx_std_20)

# Enable warnings
if (MSVC)
  target_compile_options(matrixlib PRIVATE /W4)
else()
  target_compile_options(matrixlib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Sanitizers (useful for tasks 8/10)
if (MATRIXLIB_ENABLE_SANITIZERS AND NOT MSVC)
  target_compile_options(matrixlib PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(matrixlib PRIVATE -fsanitize=address,undefined)
endif()

add_executable(matrix_console apps/console/main.cpp)
target_link_libraries(matrix_console PRIVATE matrixlib)

if (MATRIXLIB_ENABLE_SANITIZERS AND NOT MSVC)
  target_compile_options(matrix_console PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(matrix_console PRIVATE -fsanitize=address,undefined)
endif()

# Tests
if (MATRIXLIB_BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()
  add_executable(matrix_tests
    tests/test_basic.cpp
    tests/test_det.cpp
    tests/test_lu.cpp
    tests/test_perf.cpp
  )
  target_link_libraries(matrix_tests PRIVATE matrixlib GTest::gtest_main)

  if (MATRIXLIB_ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(matrix_tests PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(matrix_tests PRIVATE -fsanitize=address,undefined)
  endif()

  include(GoogleTest)
  gtest_discover_tests(matrix_tests)
endif()
