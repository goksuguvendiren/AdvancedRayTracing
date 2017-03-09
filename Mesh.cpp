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
        return std::make_pair(false, Triangle {1, 1, 1});
    }
    if (!(stream >> y)) {
        return std::make_pair(false, Triangle {1, 1, 1});
    }
    if (!(stream >> z)) {
        return std::make_pair(false, Triangle {1, 1, 1});
    }

    return std::make_pair(true, Triangle {x, y, z});
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
//            auto fc = ([&stream] {
//
//                int a, b, c;
//                stream >> a;
//                stream >> b;
//                stream >> c;
//
//                return Triangle {a, b, c};
//
//            })();

            m.AddFace(tr.second);
        }
        meshes.push_back(m);
    }

    return meshes;
}

std::pair<bool, HitInfo> Mesh::Hit(const Ray &ray)
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