//
// Created by Göksu Güvendiren on 24/02/2017.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include "Camera.h"
#include "Sphere.h"
#include "Triangle.h"

class Scene
{
    std::vector<Camera> cameras;
    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Material> materials;

public:
    const std::vector<Camera>& Cameras() const { return cameras; }
    const std::vector<Sphere>& Spheres() const { return spheres; }
    const std::vector<Triangle>& Triangles() const { return triangles; }
    const std::vector<Material>& Materials() const { return materials; }

    void AddCamera(Camera&& cam) { cameras.push_back(std::move(cam)); }
    void AddCamera(const Camera& cam) { cameras.push_back(cam); }

    void AddSphere(Sphere&& sph) { spheres.push_back(std::move(sph)); }
    void AddSphere(const Sphere& sph) { spheres.push_back(sph); }

    void AddTriangle(Triangle&& tri) { triangles.push_back(std::move(tri)); }
    void AddTriangle(const Triangle& tri) { triangles.push_back(tri); }

    void AddMaterial(Material&& mat) { materials.push_back(std::move(mat)); }
    void AddMaterial(const Material& mat) { materials.push_back(mat); }

    Material GetMaterial(unsigned int id) { return materials[id - 1]; }
};

Scene CreateScene();

#endif //RAYTRACER_SCENE_H
