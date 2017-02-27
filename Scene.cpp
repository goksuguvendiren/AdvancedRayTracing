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

    return scene;
}