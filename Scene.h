//
// Created by Göksu Güvendiren on 24/02/2017.
//

#pragma once

#include <vector>
#include <map>
#include "Camera.h"
#include "Sphere.h"
#include "Vertex.h"

class Triangle;
class Sphere;
class Mesh;
class LightSource;
class Shape;

class Scene
{
    glm::vec3 backgroundColor;
    glm::vec3 ambientLight;
    float shadowRayEpsilon;
    float intersectionTestEpsilon;

    std::vector<Shape*> shapes;

    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Mesh> meshes;

    std::vector<Material> materials;
    std::vector<glm::vec3> vertices;
    std::map<std::string, glm::mat4> transformations;
    std::vector<LightSource> lights;
    std::vector<Camera> cameras;

public:
    Scene(glm::vec3 bg = {0, 0, 0}, glm::vec3 al = {0, 0, 0});
    ~Scene();
    Scene(const Scene& s) = delete;
    Scene& operator=(const Scene&) = delete;

    const std::vector<Shape*>& Shapes() const;

    glm::mat4 GetTransformation(std::string str) const;

    glm::vec3 BackgroundColor() const;
    void BackgroundColor(glm::vec3 bg);

    glm::vec3 AmbientLight() const;
    void AmbientLight(glm::vec3 ac);

    float ShadowRayEpsilon() const;
    void ShadowRayEpsilon(float sre);

    float IntersectionTestEpsilon() const;
    void IntersectionTestEpsilon(float ite);

    const std::vector<Camera>& Cameras() const;
    const std::vector<Sphere>& Spheres() const;
    const std::vector<Triangle>& Triangles() const;
    const std::vector<Material>& Materials() const;
    const std::vector<Mesh>& Meshes() const;
    const std::vector<LightSource>& Lights() const;

    void AddCamera(Camera&& cam);
    void AddCamera(const Camera& cam);

    void AddSphere(Sphere&& sph);
    void AddSphere(const Sphere& sph);

    void AddTriangle(Triangle&& tri);
    void AddTriangle(const Triangle& tri);

    void AddMaterial(Material&& mat);
    void AddMaterial(const Material& mat);

    void AddVertex(glm::vec3&& vert);
    void AddVertex(const glm::vec3& vert);

    const Material& GetMaterial(int id);
    glm::vec3 GetVertex(int id);

    void CreateScene(std::string filename);
};

extern Scene scene;
