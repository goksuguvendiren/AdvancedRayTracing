//
// Created by Göksu Güvendiren on 24/02/2017.
//

#pragma once

#include <vector>
#include <map>
#include <memory>
#include "Camera.h"
#include "Shapes/Sphere.h"
#include "Vertex.h"
#include "BoundingVolume.h"

class Triangle;
class Sphere;
class Mesh;
class Light;
class Shape;
class Texture;
class Materialx;
class BRDF;

class Scene
{
    std::string path;

    glm::vec3 backgroundColor;
    glm::vec3 ambientLight;
    float shadowRayEpsilon;
    float intersectionTestEpsilon;

    int maxRecDepth;

    std::vector<Shape*> shapes;

    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Mesh> meshes;
    std::vector<Mesh> meshInstances;

    std::map<int, Material> materials;
    std::vector<Vertex> vertices;
    std::vector<glm::vec2> texCoords;

    std::map<std::string, glm::mat4> transformations;
    std::vector<std::unique_ptr<Light>> lights;
    std::vector<std::unique_ptr<BRDF>> brdfs;
    std::vector<Camera> cameras;
    std::vector<Texture> textures;

    BoundingVolume boundingBox;

public:
    Scene(glm::vec3 bg = {0, 0, 0}, glm::vec3 al = {0, 0, 0});
    ~Scene();
    Scene(const Scene& s) = delete;
    Scene& operator=(const Scene&) = delete;

    void SetPath(const std::string& p) { path = p; }
    std::string GetPath() const { return path; }

    const std::vector<Shape*>& Shapes() const;

    glm::mat4 GetTransformation(std::string str) const;

    glm::vec3 BackgroundColor() const;
    void BackgroundColor(glm::vec3 bg);

    glm::vec3 AmbientLight() const;
    void AmbientLight(glm::vec3 ac);

    float ShadowRayEpsilon() const;
    void ShadowRayEpsilon(float sre);

    int MaxRecursionDepth() const;
    void MaxRecursionDepth(int mrd);

    float IntersectionTestEpsilon() const;
    void IntersectionTestEpsilon(float ite);

    const std::vector<Camera>& Cameras() const;
    const std::vector<Sphere>& Spheres() const;
    const std::vector<Triangle>& Triangles() const;
    const std::vector<Material>& Materials() const;
    const std::vector<Mesh>& Meshes() const;
    const std::vector<std::unique_ptr<Light>>& Lights() const;

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

    const Material& GetMaterial(int id);
    const BRDF* GetBRDF(int id);
    const Texture&  GetTexture(int id);
    const Triangle& GetTriangle(int id);
    Vertex& GetVertex(int id);
    glm::vec2 Get_UV(int id) const;

    boost::optional<HitInfo> Hit(const Ray& r);
    boost::optional<float>   ShadowHit(const Ray& ray);
    
    const BoundingVolume& BoundingBox();

    void CreateScene(std::string filename);
};

extern Scene scene;
