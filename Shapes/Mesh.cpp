//
// Created by Göksu Güvendiren on 06/03/2017.
//

#include <sstream>
#include "Mesh.h"
#include "../Materials/LightMaterial.hpp"

inline boost::optional<Triangle> GetFace(std::istringstream& stream, int vertexOffset, int texCoordsOffset,
                                         const glm::mat4& matrix, int matID, int index, bool smooth, int texID)
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

    int a = x;
    int b = y;
    int c = z;

    x += vertexOffset;
    y += vertexOffset;
    z += vertexOffset;

    auto ind0 = glm::vec4(scene.GetVertex(x).Data(), 1);
    auto ind1 = glm::vec4(scene.GetVertex(y).Data(), 1);
    auto ind2 = glm::vec4(scene.GetVertex(z).Data(), 1);

    ind0 = matrix * ind0;
    ind1 = matrix * ind1;
    ind2 = matrix * ind2;

    if (texID != -1 && !scene.GetTexture(texID).IsPerlin())
    {
        return Triangle{Vertex{x, {ind0.x, ind0.y, ind0.z}, {0, 0, 0}, scene.Get_UV(a + texCoordsOffset)},
                        Vertex{y, {ind1.x, ind1.y, ind1.z}, {0, 0, 0}, scene.Get_UV(b + texCoordsOffset)},
                        Vertex{z, {ind2.x, ind2.y, ind2.z}, {0, 0, 0}, scene.Get_UV(c + texCoordsOffset)},
                        scene.GetMaterial(matID), texID, index, smooth};
    }
    else
    {
        return Triangle{Vertex{x, {ind0.x, ind0.y, ind0.z}},
                        Vertex{y, {ind1.x, ind1.y, ind1.z}},
                        Vertex{z, {ind2.x, ind2.y, ind2.z}},
                        scene.GetMaterial(matID), texID, index, smooth};
    }
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

    for (auto child = elem->FirstChildElement("Mesh"); child != nullptr; child = child->NextSiblingElement("Mesh")) {
        int id;
        child->QueryIntAttribute("id", &id);

        int matID = child->FirstChildElement("Material")->IntText(0);
        int texID = -1;
        if (auto sth = child->FirstChildElement("Texture")){
            texID = sth->IntText(0);
        }

        std::vector<std::string> transformations;
        if(auto trns = child->FirstChildElement("Transformations")){
            std::istringstream ss {trns->GetText()};
            transformations = GetTransformations(ss);
        }

        glm::mat4 matrix;
        for (auto& tr : transformations){
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        Mesh msh {id, scene.GetMaterial(matID)};
        auto FaceData = child->FirstChildElement("Faces");
        std::istringstream stream { FaceData->GetText() };
        int vertexOffset = 0;
        int texCoordOffset = 0;

        if (FaceData->QueryIntAttribute("vertexOffset", &vertexOffset));
        if (FaceData->QueryIntAttribute("textureOffset", &texCoordOffset));

        boost::optional<Triangle> tr;
        int index = 1;
        
        ShadingMode mode = ShadingMode::Flat;
        const char* asd = child->Attribute("shadingMode");
        if (asd != nullptr){
            std::string sm = std::string(asd);
            if (sm == "smooth") {
                mode = ShadingMode::Smooth;
            }
        }
        
        while((tr = GetFace(stream, vertexOffset, texCoordOffset, matrix, matID, index++, (mode == ShadingMode::Smooth), texID))){
            auto tri = *tr;
            msh.AddFace(std::move(*tr));
        }

        msh.SetShadingMode(mode);

        msh.BoundingBox();
        meshes.push_back(std::move(msh));
        
    }
    
    
    for (auto& mesh : meshes)
    {
        assert(mesh.GetMaterial());
    }
    
    return meshes;
}

boost::optional<HitInfo> Mesh::Hit(const Ray &ray) const
{
    auto res = volume.Hit(ray);
    if (res)
    {
        res->SetShape(this);
        assert(res->GetMaterial());
    }
    
    return res;
};

boost::optional<float> Mesh::ShadowHit(const Ray& ray) const
{
    return volume.ShadowHit(ray);
};

void Mesh::InsertVT(Triangle face)
{
    vertex_triangle_associtations.insert(std::make_pair(face.PointA().ID(), face.ID()));
    vertex_triangle_associtations.insert(std::make_pair(face.PointB().ID(), face.ID()));
    vertex_triangle_associtations.insert(std::make_pair(face.PointC().ID(), face.ID()));
}

void Mesh::SetShadingMode(enum ShadingMode mode)
{
    shmode = mode;
    switch(shmode){
        case ShadingMode::Smooth:
//            AssociateV2T();
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

void Mesh::BoundingBox()
{
    volume = BoundingVolume(faces, Axis::X);
}

//void Mesh::AssociateV2T()
//{
//    for (auto& face : faces){
//        InsertVT(face);
//    }
//
//    for (auto& face : faces){
//        SetNormal(face.PointA());
//        SetNormal(face.PointB());
//        SetNormal(face.PointC());
//    }
//}

glm::vec2 Mesh::GetTexCoords(glm::vec3 pos) const
{
    return glm::vec2();
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
            transformations = GetTransformations(ss);
        }

        glm::mat4 matrix;
        for (auto& tr : transformations){
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        auto& baseMesh = scene.Meshes()[baseID - 1];
        assert(baseMesh.ID() == baseID);

        Mesh mesh {id, baseMesh.GetMaterial()};
        int index = 1;
        for (auto& face : baseMesh.Faces()){
            auto vert0 = matrix * glm::vec4(face.PointA().Data(), 1);
            auto vert1 = matrix * glm::vec4(face.PointB().Data(), 1);
            auto vert2 = matrix * glm::vec4(face.PointC().Data(), 1);

            auto vertex0 = Vertex{face.PointA().ID(), {vert0.x, vert0.y, vert0.z}};
            auto vertex1 = Vertex{face.PointB().ID(), {vert1.x, vert1.y, vert1.z}};
            auto vertex2 = Vertex{face.PointC().ID(), {vert2.x, vert2.y, vert2.z}};

            auto tri = Triangle{vertex0, vertex1, vertex2, mesh.GetMaterial(), -1,
                                index++, baseMesh.GetShadingMode() == ShadingMode::Smooth};
            mesh.AddFace(std::move(tri));
        }

        mesh.SetShadingMode(baseMesh.GetShadingMode());

        meshes.push_back(std::move(mesh));
    }

    return meshes;
}
