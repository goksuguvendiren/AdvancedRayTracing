cmake_minimum_required(VERSION 3.6)
project(IBL)

find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++14 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++14 -O3")

set(SOURCE_FILES IBL_PostProcess.cpp)

add_executable(IBL ${SOURCE_FILES})

target_link_libraries(IBL ${Boost_LIBRARIES})
target_link_libraries(IBL ${OpenCV_LIBS})
target_include_directories(IBL PUBLIC ${Boost_INCLUDE_DIRS})
