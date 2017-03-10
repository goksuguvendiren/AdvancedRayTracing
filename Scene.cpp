//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include "Scene.h"
#include "LightSource.h"
#include "Triangle.h"
#include "Mesh.h"
#include "tinyxml/tinyxml2.h"
#include <sstream>

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
        camera = CreateCamera(elem);
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
        ls = CreateLights(elem);
    }

    std::vector<Material> mats;
    if (auto elem = docscene->FirstChildElement("Materials")){
        mats = CreateMaterials(elem);
    }

    std::vector<Vertex> verts;
    if (auto elem = docscene->FirstChildElement("VertexData")){
        verts = CreateVertexData(elem);
    }

    vertices = std::move(verts);

    std::vector<Triangle> tris;
    std::vector<Sphere> sphs;
    std::vector<Mesh> mshs;
    if(auto objects = docscene->FirstChildElement("Objects")){
        tris = CreateTriangles(objects);
        sphs = CreateSpheres(objects);
        mshs = CreateMeshes(objects);
    }

    AddCamera(camera);

    materials = std::move(mats);
    triangles = std::move(tris);
    spheres   = std::move(sphs);
    meshes    = std::move(mshs);
    lights    = std::move(ls);
}

Scene::Scene(glm::vec3 bg, glm::vec3 al) : backgroundColor(bg), ambientLight(al)
{
    shadowRayEpsilon = 1e-3;
    intersectionTestEpsilon = - 1e-6;
}

Scene::~Scene() {}

glm::vec3 Scene::BackgroundColor()
{ return backgroundColor; }

void Scene::BackgroundColor(glm::vec3 bg)
{ backgroundColor = bg; }

glm::vec3 Scene::AmbientLight()
{ return ambientLight; }

void Scene::AmbientLight(glm::vec3 al)
{ ambientLight = al; }

float Scene::ShadowRayEpsilon()
{ return shadowRayEpsilon; }

void Scene::ShadowRayEpsilon(float sre)
{ shadowRayEpsilon = sre; }

float Scene::IntersectionTestEpsilon()
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

void Scene::AddVertex(Vertex&& vert) { vertices.push_back(std::move(vert)); }
void Scene::AddVertex(const Vertex& vert) { vertices.push_back(vert); }

Material Scene::GetMaterial(unsigned int id) { return materials[id - 1]; }
Vertex Scene::GetVertex(unsigned int id) { return vertices[id - 1]; }

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
