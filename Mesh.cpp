//
// Created by Göksu Güvendiren on 06/03/2017.
//

#include <sstream>
#include "Mesh.h"

inline std::pair<bool, Triangle> GetFace(std::istringstream& stream, const glm::mat4& matrix)
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

    auto ind0 = glm::vec4(scene.GetVertex(x), 1);
    auto ind1 = glm::vec4(scene.GetVertex(y), 1);
    auto ind2 = glm::vec4(scene.GetVertex(z), 1);

    ind0 = matrix * ind0;
    ind1 = matrix * ind1;
    ind2 = matrix * ind2;

    return std::make_pair(true, Triangle{ind0, ind1, ind2});
}

inline auto GetTransformations(std::istringstream& stream)
{
    std::vector<std::string> result;

    while(stream.good()){
        std::string tr;
        stream >> tr;
        result.push_back(tr);
    }

    return result;
}

std::vector<Mesh> CreateMeshes(tinyxml2::XMLElement* elem)
{
    std::vector<Mesh> meshes;

    for (auto child = elem->FirstChildElement("Mesh"); child != NULL; child = child->NextSiblingElement("Mesh")) {
        int id;
        child->QueryIntAttribute("id", &id);

        int matID = child->FirstChildElement("Material")->IntText(0);

        std::vector<std::string> transformations;
        if(auto trns = child->FirstChildElement("Transformations")){
            std::istringstream ss {trns->GetText()};
            transformations = std::move(GetTransformations(ss));
        }

        glm::mat4 matrix;
        for (auto& tr : transformations){
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        Mesh msh {id, matID};
        std::istringstream stream { child->FirstChildElement("Faces")->GetText() };

        std::pair<bool, Triangle> tr;
        while((tr = GetFace(stream, matrix)).first){
            msh.AddFace(tr.second);
        }

        meshes.push_back(std::move(msh));
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