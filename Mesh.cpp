//
// Created by Göksu Güvendiren on 06/03/2017.
//

#include <sstream>
#include "Mesh.h"

inline boost::optional<Triangle> GetFace(std::istringstream& stream, const glm::mat4& matrix, int matID, int index, bool smooth)
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
                    matID, index, smooth};
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

std::vector<Mesh> LoadMeshInstances(tinyxml2::XMLElement *elem)
{
    std::vector<Mesh> meshes;
    for (auto child = elem->FirstChildElement("MeshInstance"); child != nullptr; child = child->NextSiblingElement("MeshInstance")){
        int id;
        child->QueryIntAttribute("id", &id);
        int baseID;
        child->QueryIntAttribute("baseMeshId", &baseID);

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

        auto& baseMesh = scene.Meshes()[baseID - 1];
        assert(baseMesh.ID() == baseID);

        Mesh mesh {id, baseMesh.MaterialID()};
        int index = 1;
        for (auto& face : baseMesh.Faces()){
            auto vert0 = matrix * glm::vec4(face.PointA().Data(), 1);
            auto vert1 = matrix * glm::vec4(face.PointB().Data(), 1);
            auto vert2 = matrix * glm::vec4(face.PointC().Data(), 1);

            auto vertex0 = Vertex{face.PointA().ID(), {vert0.x, vert0.y, vert0.z}};
            auto vertex1 = Vertex{face.PointB().ID(), {vert1.x, vert1.y, vert1.z}};
            auto vertex2 = Vertex{face.PointC().ID(), {vert2.x, vert2.y, vert2.z}};

            auto tri = Triangle{vertex0, vertex1, vertex2, mesh.MaterialID(), index++, baseMesh.ShadingMode() == ShadingMode::Smooth};
            mesh.AddFace(std::move(tri));
        }

        mesh.ShadingMode(baseMesh.ShadingMode());

        meshes.push_back(std::move(mesh));
    }

    return meshes;
}

std::vector<Mesh> LoadMeshes(tinyxml2::XMLElement *elem)
{
    std::vector<Mesh> meshes;

    for (auto child = elem->FirstChildElement("Mesh"); child != nullptr; child = child->NextSiblingElement("Mesh")) {
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
        while((tr = GetFace(stream, matrix, matID, index++, ShadingMode() == ShadingMode::Smooth))){
            msh.AddFace(std::move(*tr));
        }

        const char* asd = child->Attribute("shadingMode");
        if (asd != nullptr){
            std::string sm = std::string(asd);
            if (sm == "smooth") {
                msh.ShadingMode(ShadingMode::Smooth);
            }
        }
        else {
            msh.ShadingMode(ShadingMode::Flat);
        }

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

void Mesh::ShadingMode(enum ShadingMode mode)
{
    shmode = mode;
    switch(shmode){
        case ShadingMode::Smooth:
            AssociateV2T();
            break;
        case ShadingMode::Flat:
        default:
                break;
    }
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