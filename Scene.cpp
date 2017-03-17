//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include "Scene.h"
#include "LightSource.h"
#include "Triangle.h"
#include "Transformation.h"
#include "Mesh.h"
#include "tinyxml/tinyxml2.h"
#include <sstream>
#include <map>

inline glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

void Scene::CreateScene(std::string filename)
{
    tinyxml2::XMLDocument document;
    document.LoadFile(filename.c_str());

    if (document.Error()){
        document.PrintError();
        std::abort();
    }

    auto docscene = document.FirstChildElement("Scene");

    if (!docscene){
        std::cerr << "Not a valid scene file" << '\n';
        std::abort();
    }

    if (auto elem = docscene->FirstChildElement("BackgroundColor")){
        auto color = GetElem(elem);
        BackgroundColor(color);
    }

    if (auto elem = docscene->FirstChildElement("ShadowRayEpsilon")){
        ShadowRayEpsilon(elem->FloatText());
    }

    if (auto elem = docscene->FirstChildElement("IntersectionTestEpsilon")){
        IntersectionTestEpsilon(elem->FloatText());
    }

    Camera camera;
    if (auto elem = docscene->FirstChildElement("Camera")){
        camera = LoadCamera(elem);
    }
    else {
        std::cerr << "Could not read camera information\n";
        std::abort();
    }

    std::vector<LightSource> ls;
    if (auto elem = docscene->FirstChildElement("Lights")){
        if (auto al = elem->FirstChildElement("AmbientLight")){
            auto ambient = GetElem(al);
            AmbientLight(ambient);
        }
        lights = LoadLights(elem);
    }

    if (auto elem = docscene->FirstChildElement("Materials")){
        materials = LoadMaterials(elem);
    }

    if (auto elem = docscene->FirstChildElement("VertexData")){
        vertices = LoadVertexData(elem);
    }

    if (auto elem = docscene->FirstChildElement("Transformations")){
        transformations  = LoadTransformations(elem);
    }

    std::vector<Triangle> tris;
    std::vector<Sphere> sphs;
    std::vector<Mesh> mshs;

    if(auto objects = docscene->FirstChildElement("Objects")){
        triangles = LoadTriangles(objects);
        spheres   = LoadSpheres(objects);
        meshes    = LoadMeshes(objects);
    }

    AddCamera(camera);

    for_each(triangles.begin(), triangles.end(), [this](auto& tri) { shapes.push_back(&tri); });
    for_each(spheres.begin(), spheres.end(), [this](auto& sph) { shapes.push_back(&sph); });
    for_each(meshes.begin(), meshes.end(), [this](auto& msh) { shapes.push_back(&msh); });
}

glm::mat4 Scene::GetTransformation(std::string str) const
{
    return transformations.find(str)->second;
}

Scene::Scene(glm::vec3 bg, glm::vec3 al) : backgroundColor(bg), ambientLight(al)
{
    shadowRayEpsilon = 1e-3;
    intersectionTestEpsilon = - 1e-6;
}

Scene::~Scene() {}


const std::vector<Shape*>& Scene::Shapes() const
{ return shapes; }

glm::vec3 Scene::BackgroundColor() const
{ return backgroundColor; }

void Scene::BackgroundColor(glm::vec3 bg)
{ backgroundColor = bg; }

glm::vec3 Scene::AmbientLight() const
{ return ambientLight; }

void Scene::AmbientLight(glm::vec3 al)
{ ambientLight = al; }

float Scene::ShadowRayEpsilon() const
{ return shadowRayEpsilon; }

void Scene::ShadowRayEpsilon(float sre)
{ shadowRayEpsilon = sre; }

float Scene::IntersectionTestEpsilon() const
{ return intersectionTestEpsilon; }

void Scene::IntersectionTestEpsilon(float ite)
{ intersectionTestEpsilon = ite; }


void Scene::AddCamera(Camera&& cam) { cameras.push_back(std::move(cam)); }
void Scene::AddCamera(const Camera& cam) { cameras.push_back(cam); }

void Scene::AddSphere(Sphere&& sph) { spheres.push_back(std::move(sph)); }
void Scene::AddSphere(const Sphere& sph) { spheres.push_back(sph); }

void Scene::AddTriangle(Triangle&& tri) { triangles.push_back(std::move(tri)); }
void Scene::AddTriangle(const Triangle& tri) { triangles.push_back(tri); }

void Scene::AddMaterial(Material&& mat) { materials.push_back(std::move(mat)); }
void Scene::AddMaterial(const Material& mat) { materials.push_back(mat); }

void Scene::AddVertex(glm::vec3&& vert) { vertices.push_back(std::move(vert)); }
void Scene::AddVertex(const glm::vec3& vert) { vertices.push_back(vert); }

const Material& Scene::GetMaterial(int id) { return materials[id - 1]; }
glm::vec3 Scene::GetVertex(int id) { return vertices[id - 1]; }

const std::vector<Camera> & Scene::Cameras() const
{
    return cameras;
}

const std::vector<Sphere>& Scene::Spheres() const
{
    return spheres;
}

const std::vector<Triangle>& Scene::Triangles() const
{
    return triangles;
}

const std::vector<Mesh>& Scene::Meshes() const
{
    return meshes;
}

const std::vector<LightSource>& Scene::Lights() const
{
    return lights;
}

const std::vector<Material>& Scene::Materials() const
{
    return materials;
}
