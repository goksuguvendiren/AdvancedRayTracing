//
// Created by Göksu Güvendiren on 06/03/2017.
//

#include <sstream>
#include "Mesh.h"
#include <boost/optional.hpp>

inline boost::optional<Triangle> GetFace(std::istringstream& stream, const glm::mat4& matrix, int matID, int index)
{
    int x, y, z;

    if (!(stream >> x)) {
        return boost::none;
    }
    if (!(stream >> y)) {
        return boost::none;
    }
    if (!(stream >> z)) {
        return boost::none;
    }

    auto ind0 = glm::vec4(scene.GetVertex(x).Data(), 1);
    auto ind1 = glm::vec4(scene.GetVertex(y).Data(), 1);
    auto ind2 = glm::vec4(scene.GetVertex(z).Data(), 1);

    ind0 = matrix * ind0;
    ind1 = matrix * ind1;
    ind2 = matrix * ind2;

    return Triangle{Vertex{x, {ind0.x, ind0.y, ind0.z}},
                    Vertex{y, {ind1.x, ind1.y, ind1.z}},
                    Vertex{z, {ind2.x, ind2.y, ind2.z}},
                    matID, index};
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

std::vector<Mesh> LoadMeshes(tinyxml2::XMLElement *elem)
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

        boost::optional<Triangle> tr;
        int index = 1;
        while((tr = GetFace(stream, matrix, matID, index++))){
            msh.AddFace(*tr);
        }

        msh.AssociateV2T();

        meshes.push_back(std::move(msh));
    }

    return meshes;
}

boost::optional<HitInfo> Mesh::Hit(const Ray &ray) const
{
    boost::optional<HitInfo> ultHit;
    for (auto& face : faces) {
        boost::optional<HitInfo> hit;
        if ((hit = face.Hit(ray)) && ( !ultHit || hit->Parameter() < ultHit->Parameter())){
            ultHit = *hit;
        }
    }

    return ultHit;
};

bool Mesh::FastHit(const Ray &ray) const
{
    for (auto& face : faces) {
        if (face.FastHit(ray)) return true;
    }

    return false;
};

void Mesh::InsertVT(Triangle face)
{
    vertex_triangle_associtations.insert(std::make_pair(face.PointA().ID(), face.ID()));
    vertex_triangle_associtations.insert(std::make_pair(face.PointB().ID(), face.ID()));
    vertex_triangle_associtations.insert(std::make_pair(face.PointC().ID(), face.ID()));
}

void Mesh::SetNormal(Vertex& vert)
{
    glm::vec3 n = {0, 0, 0};
    auto itpair = vertex_triangle_associtations.equal_range(vert.ID());
    int num = 0;
    for (auto it = itpair.first; it != itpair.second; it++){
        auto& triangle = faces[it->second - 1];
        n += triangle.Normal();
        num++;
    }

    n /= num;
    vert.Normal(n);
}

void Mesh::AssociateV2T()
{
    for (auto& face : faces){
        InsertVT(face);
    }

    for (auto& face : faces){
        SetNormal(face.PointA());
        SetNormal(face.PointB());
        SetNormal(face.PointC());
    }
}