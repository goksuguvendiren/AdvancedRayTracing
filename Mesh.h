//
// Created by Göksu Güvendiren on 06/03/2017.
//

#pragma once

#include <map>
#include <vector>
#include "Triangle.h"
#include "Box.h"

enum class ShadingMode
{
    Smooth,
    Flat
};

class BoundingVolume;

class Mesh : public Shape
{
    int id;
    int materialID;
    ShadingMode shmode;

    std::vector<Triangle> faces;
    std::multimap<int, int> vertex_triangle_associtations;

    BoundingVolume volume;

public:

    Mesh(int mid = 1, int matID = 1) : id(mid), materialID(matID), shmode(ShadingMode::Flat) {}
    Mesh(const Mesh& m) = delete;
    Mesh(Mesh&& m) = default;

    void AddFace(Triangle&& face)
    {
        faces.push_back(std::move(face));
    }

    void BoundingBox();
    void AssociateV2T();
    void InsertVT(Triangle face);
    void SetNormal(Vertex& vert);

    void ShadingMode(enum ShadingMode mode);
    enum ShadingMode ShadingMode() const { return shmode; }

    boost::optional<HitInfo> Hit(const Ray &ray) const;
    bool FastHit(const Ray &ray) const;

    int ID() const { return id; }
    const auto& Faces() const { return faces; }
    auto MaterialID() const { return materialID; }

    glm::vec3 Min() const { return volume.BBox().Min(); }
    glm::vec3 Max() const { return volume.BBox().Max(); }

    glm::vec3 Middle() const { return volume.BBox().Middle(); }
};


std::vector<Mesh> LoadMeshes(tinyxml2::XMLElement *elem);
std::vector<Mesh> LoadMeshInstances(tinyxml2::XMLElement *elem);