//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include "Scene.h"

Scene CreateScene()
{
    Scene scene;
    ImagePlane plane = ReadImagePlane();
    Camera camera(plane);

    scene.AddCamera(camera);
    scene.AddSphere({5.0f, {0, 0, 15}});
    scene.AddTriangle({{-1.0f, -1.0f, 20.0f}, {7.0f, -1.0f, 20.0f}, {3.0f, 5.0f, 20.0f}});

    return scene;
}