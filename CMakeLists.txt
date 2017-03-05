cmake_minimum_required(VERSION 3.6)
project(RayTracer)

find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(OpenCV REQUIRED)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O0")

set(SOURCE_FILES main.cpp Camera.cpp Camera.h ImagePlane.cpp ImagePlane.h Color.cpp Color.h Sphere.cpp Sphere.h Ray.cpp Ray.h Scene.cpp Scene.h Image.cpp Image.h Triangle.cpp Triangle.h HitInfo.cpp HitInfo.h Material.cpp Material.h)
add_executable(RayTracer ${SOURCE_FILES})

target_link_libraries(RayTracer ${Boost_LIBRARIES})
target_link_libraries(RayTracer ${OpenCV_LIBS})
target_include_directories(RayTracer PUBLIC ${Boost_INCLUDE_DIRS})