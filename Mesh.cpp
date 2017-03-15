//
// Created by Göksu Güvendiren on 06/03/2017.
//

#include <sstream>
#include "Mesh.h"
#include "tinyxml/tinyxml2.h"

inline std::pair<bool, Triangle> GetFace(std::istringstream& stream)
{
    int x, y, z;

    if (!(stream >> x)) {
        return std::make_pair(false, Triangle {{1, 0, 0}, {1, 0, 0}, {1, 0, 0}});
    }
    if (!(stream >> y)) {
        return std::make_pair(false, Triangle {{1, 0, 0}, {1, 0, 0}, {1, 0, 0}});
    }
    if (!(stream >> z)) {
        return std::make_pair(false, Triangle {{1, 0, 0}, {1, 0, 0}, {1, 0, 0}});
    }

    return std::make_pair(true, Triangle {scene.GetVertex(x), scene.GetVertex(y), scene.GetVertex(z)});
}

std::vector<Mesh> CreateMeshes(tinyxml2::XMLElement* elem)
{
    std::vector<Mesh> meshes;

    for (auto child = elem->FirstChildElement("Mesh"); child != NULL; child = child->NextSiblingElement("Mesh")) {
        int id;
        child->QueryIntAttribute("id", &id);

        int matID = child->FirstChildElement("Material")->IntText(0);

        Mesh m {id, matID};
        std::istringstream stream { child->FirstChildElement("Faces")->GetText() };

        std::pair<bool, Triangle> tr;
        while((tr = GetFace(stream)).first){
            m.AddFace(tr.second);
        }
        meshes.push_back(std::move(m));
    }

    return meshes;
}

std::pair<bool, HitInfo> Mesh::Hit(const Ray &ray) const
{
    HitInfo ultHit;
    for (auto& face : faces) {
        std::pair<bool, HitInfo> hit;
        if ((hit = face.Hit(ray)).first && hit.second.Parameter() < ultHit.Parameter()){
            ultHit = hit.second;
        }
    }

    return std::make_pair(true, ultHit);
};

bool Mesh::FastHit(const Ray &ray) const
{
    for (auto& face : faces) {
        if (face.FastHit(ray)) return true;
    }

    return false;
};