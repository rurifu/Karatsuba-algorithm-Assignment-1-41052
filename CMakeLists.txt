cmake_minimum_required(VERSION 3.10)
project(karatsuba_project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(CorrectnessTest MultiplyingAlgorithm.cpp CorrectnessTest.cpp)
add_executable(BenchmarkHarness MultiplyingAlgorithm.cpp BenchmarkHarness.cpp)