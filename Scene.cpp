//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include "Scene.h"
#include "Lights/Light.h"
#include "Shapes/Triangle.h"
#include "Transformation.h"
#include "Shapes/Mesh.h"
#include "BoundingVolume.h"
#include "Materials/BRDF.hpp"
#include "tinyxml/tinyxml2.h"
#include <sstream>
#include <map>
#include "Texture.hpp"

static glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

void Compare(const glm::vec3 &val, glm::vec3& minval, glm::vec3& maxval)
{
    minval = glm::min(minval, val);
    maxval = glm::max(maxval, val);
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

    if (auto elem = docscene->FirstChildElement("MaxRecursionDepth")){
        MaxRecursionDepth(elem->IntText(1));
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

    if (auto elem = docscene->FirstChildElement("Lights")){
        if (auto al = elem->FirstChildElement("AmbientLight")){
            auto ambient = GetElem(al);
            AmbientLight(ambient);
        }
        lights = LoadLights(elem);
    }
    
    if (auto elem = docscene->FirstChildElement("BRDFs"))
    {
        brdfs = LoadBRDFs(elem);
    }

    if (auto elem = docscene->FirstChildElement("Materials")){
        materials = LoadMaterials(elem);
    }
    
    if (auto elem = docscene->FirstChildElement("TexCoordData")){
        texCoords = LoadTexCoordData(elem);
    }

    if (auto elem = docscene->FirstChildElement("VertexData")){
        vertices = LoadVertexData(elem, texCoords);
    }

    if (auto elem = docscene->FirstChildElement("Transformations")){
        transformations  = LoadTransformations(elem);
    }

    if (auto elem = docscene->FirstChildElement("Textures"))
    {
        textures = LoadTextures(elem);
    }

    if(auto objects = docscene->FirstChildElement("Objects")){
        triangles     = LoadTriangles(objects);
        spheres       = LoadSpheres(objects);
        meshes        = LoadMeshes(objects);
        meshInstances = LoadMeshInstances(objects);
    }

    AddCamera(camera);

    glm::vec3 mins, maxs;

    for_each(triangles.begin(), triangles.end(), [this, &mins, &maxs](auto& tri) {
        Compare(tri.Max(), mins, maxs);
        Compare(tri.Min(), mins, maxs);
        shapes.push_back(&tri);
    });

    for_each(meshes.begin(), meshes.end(), [this, &mins, &maxs](auto& msh) {
        Compare(msh.Max(), mins, maxs);
        Compare(msh.Min(), mins, maxs);

        shapes.push_back(&msh);
    });

    for_each(meshInstances.begin(), meshInstances.end(), [this, &mins, &maxs](auto& msh) {
        Compare(msh.Max(), mins, maxs);
        Compare(msh.Min(), mins, maxs);

        shapes.push_back(&msh);
    });
    
    if (shapes.size() > 0)
        boundingBox = BoundingVolume(shapes, Axis::X);
    else boundingBox = BoundingVolume();
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

int Scene::MaxRecursionDepth() const
{ return maxRecDepth; }

void Scene::MaxRecursionDepth(int mrd)
{ maxRecDepth = mrd; }

float Scene::IntersectionTestEpsilon() const
{ return intersectionTestEpsilon; }

void Scene::IntersectionTestEpsilon(float ite)
{ intersectionTestEpsilon = ite; }

const BoundingVolume& Scene::BoundingBox()
{
    return boundingBox;
}

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

const Material& Scene::GetMaterial(int id) { return materials[id - 1]; }
const BRDF* Scene::GetBRDF(int id) { return brdfs[id - 1].get(); }

const Texture&  Scene::GetTexture(int id)  { return textures[id - 1]; }
const Triangle& Scene::GetTriangle(int id) { return triangles[id - 1]; }

Vertex& Scene::GetVertex(int id) { return vertices[id - 1]; }
glm::vec2 Scene::Get_UV(int id) const
{
    return texCoords[id - 1];
}

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

const std::vector<std::unique_ptr<Light>>& Scene::Lights() const
{
    return lights;
}

const std::vector<Material>& Scene::Materials() const
{
    return materials;
}

boost::optional<float> Scene::ShadowHit(const Ray& ray)
{
    auto bvhHit = BoundingBox().ShadowHit(ray);
    
    boost::optional<float> sphHit;
    for (auto& sph : scene.Spheres()){
        auto tmpHit = sph.ShadowHit(ray);
        if (!sphHit || (tmpHit && *sphHit > *tmpHit))
        {
            sphHit = tmpHit;
        }
    }
    
    if (bvhHit && sphHit)
    {
        return *bvhHit < *sphHit ? bvhHit : sphHit;
    }
    
    if (!sphHit && !bvhHit)
    {
        return boost::none;
    }
    
    return sphHit ? sphHit : bvhHit;
}

boost::optional<HitInfo> Scene::Hit(const Ray &r)
{
    auto bvhHit = BoundingBox().Hit(r);

    boost::optional<HitInfo> sphHit;
    for (auto& sph : scene.Spheres()){
        auto tmpHit = sph.Hit(r);
        if (!sphHit || (tmpHit && sphHit->Parameter() > tmpHit->Parameter()))
        {
            sphHit = tmpHit;
        }
    }

    if (bvhHit && sphHit)
    {
        return bvhHit->Parameter() < sphHit->Parameter() ? bvhHit : sphHit;
    }

    if (!sphHit && !bvhHit)
    {
        return boost::none;
    }

    return sphHit ? sphHit : bvhHit;
}
