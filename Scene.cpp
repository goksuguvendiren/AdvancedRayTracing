//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include "Scene.h"
#include "tinyxml/tinyxml2.h"

Scene CreateScene(std::string filename)
{
    tinyxml2::XMLDocument document;
    document.LoadFile(filename.c_str());

    if (document.Error()){
        document.PrintError();
        std::abort();
    }

    Scene scene;
    ImagePlane plane = ReadImagePlane();
    Camera camera(plane, {0, 5, 25}, {0, 0, -1}, {0, 1, 0});

    scene.AddCamera(camera);
//    scene.AddSphere({5.0f, {0, 5.0f, 0.0f}});
    scene.AddTriangle({{100.0f, 0.0f, -100.0f}, {-100.0f, 0.0f,  100.0f}, {100.0f, 0.0f,  100.0f}});

    return scene;
}