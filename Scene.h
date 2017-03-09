//
// Created by Göksu Güvendiren on 24/02/2017.
//

#pragma once

#include <vector>
#include "Camera.h"
#include "Sphere.h"
#include "Vertex.h"

class Triangle;
class Sphere;
class Mesh;

class Scene
{
    glm::vec3 backgroundColor;
    float shadowRayEpsilon;
    float intersectionTestEpsilon;

    std::vector<Camera> cameras;
    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Material> materials;
    std::vector<Vertex> vertices;
    std::vector<Mesh> meshes;

public:
    Scene(glm::vec3 bg = {0, 0, 0});
    ~Scene();

    Scene& operator=(const Scene&);

    glm::vec3 BackgroundColor();
    void BackgroundColor(glm::vec3 bg);

    float ShadowRayEpsilon();
    void ShadowRayEpsilon(float sre);

    float IntersectionTestEpsilon();
    void IntersectionTestEpsilon(float ite);

    const std::vector<Camera>& Cameras() const;
    const std::vector<Sphere>& Spheres() const;
    const std::vector<Triangle>& Triangles() const;
    const std::vector<Material>& Materials() const;
    const std::vector<Mesh>& Meshes() const;

    void AddCamera(Camera&& cam);
    void AddCamera(const Camera& cam);

    void AddSphere(Sphere&& sph);
    void AddSphere(const Sphere& sph);

    void AddTriangle(Triangle&& tri);
    void AddTriangle(const Triangle& tri);

    void AddMaterial(Material&& mat);
    void AddMaterial(const Material& mat);

    void AddVertex(Vertex&& vert);
    void AddVertex(const Vertex& vert);

    Material GetMaterial(unsigned int id);
    Vertex GetVertex(unsigned int id);

    void CreateScene(std::string filename);
};

extern Scene scene;
