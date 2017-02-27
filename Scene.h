//
// Created by Göksu Güvendiren on 24/02/2017.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include "Camera.h"
#include "Sphere.h"

class Scene
{
    std::vector<Camera> cameras;
    std::vector<Sphere> spheres;

public:
    const std::vector<Camera>& Cameras() const { return cameras; }
    const std::vector<Sphere>& Spheres() const { return spheres; }

    void AddCamera(const Camera& cam) { cameras.push_back(std::move(cam)); }
    void AddSphere(const Sphere& sph) { spheres.push_back(std::move(sph)); }
};

Scene CreateScene();

#endif //RAYTRACER_SCENE_H
