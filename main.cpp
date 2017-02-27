#include <iostream>

#include "Camera.h"
#include "Scene.h"

Scene scene;

int main()
{
    scene = CreateScene();

    std::vector<Image> images;

    for (auto& cam : scene.Cameras()){
        images.push_back(cam.Render());
    }

    return 0;
}