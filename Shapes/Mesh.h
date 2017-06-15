//
// Created by Göksu Güvendiren on 06/03/2017.
//

#pragma once

#include <map>
#include <vector>
#include "Triangle.h"
#include "../Box.h"

enum class ShadingMode
{
    Smooth,
    Flat
};

class BoundingVolume;

class Mesh : public Shape
{
    int id;
    const Materialx* material;
    const Texture*   texture;
    ShadingMode shmode;

    std::vector<Triangle> faces;
    std::multimap<int, int> vertex_triangle_associtations;

    BoundingVolume volume;
    bool is_artificial;

public:

    Mesh(int mid = 1, const Materialx* mat = nullptr) : id(mid), material(mat), shmode(ShadingMode::Flat)
    {}
    
    Mesh(const Mesh& m) = delete;
    Mesh(Mesh&& m) = default;

    void AddFace(Triangle&& face)
    {
        faces.push_back(std::move(face));
    }

    void BoundingBox();
//    void AssociateV2T();
    void InsertVT(Triangle face);
    void SetNormal(Vertex& vert);

    void SetShadingMode(enum ShadingMode mode);
    enum ShadingMode GetShadingMode() const { return shmode; }

    boost::optional<HitInfo> Hit(const Ray &ray) const;
    boost::optional<float>   ShadowHit(const Ray& ray) const;

    int ID() const { return id; }
    const auto& Faces() const { return faces; }

    auto GetMaterial() const { return material; }
//    auto GetTexture()  const { return texture; }

    glm::vec3 Min() const { return volume.BBox().Min(); }
    glm::vec3 Max() const { return volume.BBox().Max(); }

    glm::vec3 Middle() const { return volume.BBox().Middle(); }

    glm::vec2 GetTexCoords(glm::vec3 pos) const;
    
    bool isArtificial() const { return is_artificial; }
    void SetArtificial(bool art) { is_artificial = art; }
};


std::vector<Mesh> LoadMeshes(tinyxml2::XMLElement *elem);
std::vector<Mesh> LoadMeshInstances(tinyxml2::XMLElement *elem);
