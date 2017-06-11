cmake_minimum_required(VERSION 3.6)
project(RayTracer)

find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++14 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++14 -O3")

set(SOURCE_FILES main.cpp Camera.cpp Camera.h ImagePlane.cpp ImagePlane.h Color.h Shapes/Sphere.cpp Shapes/Sphere.h
        Ray.h Scene.cpp Scene.h Image.h Shapes/Triangle.cpp Shapes/Triangle.h HitInfo.h Material.cpp
        Material.h tinyxml/tinyxml2.cpp tinyxml/tinyxml2.h Vertex.cpp Vertex.h Shapes/Mesh.cpp Shapes/Mesh.h
        Lights/PointLight.h Shapes/Shape.h Transformation.cpp Transformation.h Box.cpp Box.h BoundingVolume.cpp
        BoundingVolume.h Lights/Light.h Lights/Light.cpp Lights/SpotLight.h Lights/AreaLight.cpp Lights/AreaLight.h
        Texture.cpp Texture.hpp PerlinNoise.cpp PerlinNoise.hpp Lights/DirectionalLight.cpp Lights/DirectionalLight.hpp
        Materials/BlinnPhongBRDF.cpp Materials/BlinnPhongBRDF.hpp Materials/PhongBRDF.cpp Materials/PhongBRDF.hpp 
		Materials/Materialx.cpp Materials/Materialx.hpp Materials/ClassicMaterial.cpp Materials/ClassicMaterial.hpp
		Materials/BRDF.hpp Materials/BRDF.cpp Materials/BlinnPhongBRDF_Mdf.cpp Materials/BlinnPhongBRDF_Mdf.hpp
		Materials/PhongBRDF_Mdf.cpp Materials/PhongBRDF_Mdf.hpp Materials/TorranceSparrow.cpp Materials/TorranceSparrow.hpp
Materials/LightMaterial.hpp Materials/Mirror.cpp Materials/Mirror.hpp ColorCalculation.cpp ColorCalculation.h Materials/Glass.cpp Materials/Glass.hpp Materials/LightSource.cpp Materials/LightSource.h)

add_executable(RayTracer ${SOURCE_FILES})

target_link_libraries(RayTracer ${Boost_LIBRARIES})
target_link_libraries(RayTracer ${OpenCV_LIBS})
target_include_directories(RayTracer PUBLIC ${Boost_INCLUDE_DIRS})
