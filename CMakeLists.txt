cmake_minimum_required(VERSION 3.6)
project(RayTracer)

find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(OpenCV REQUIRED)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3")

set(SOURCE_FILES main.cpp Camera.cpp Camera.h ImagePlane.cpp ImagePlane.h Color.h Sphere.cpp Sphere.h
        Ray.h Scene.cpp Scene.h Image.h Triangle.cpp Triangle.h HitInfo.h Material.cpp
        Material.h tinyxml/tinyxml2.cpp tinyxml/tinyxml2.h Vertex.cpp Vertex.h Mesh.cpp Mesh.h
        Lights/PointLight.h Shape.h Transformation.cpp Transformation.h Box.cpp Box.h BoundingVolume.cpp
        BoundingVolume.h Lights/Light.h Lights/Light.cpp Lights/SpotLight.h)

add_executable(RayTracer ${SOURCE_FILES})

target_link_libraries(RayTracer ${Boost_LIBRARIES})
target_link_libraries(RayTracer ${OpenCV_LIBS})
target_include_directories(RayTracer PUBLIC ${Boost_INCLUDE_DIRS})