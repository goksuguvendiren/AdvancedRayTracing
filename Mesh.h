//
// Created by Göksu Güvendiren on 06/03/2017.
//

#pragma once

#include <map>
#include <vector>
#include "Triangle.h"

enum class ShadingMode
{
    Smooth,
    Flat
};

class Mesh : public Shape
{
    int id;
    int materialID;
    ShadingMode shmode;

    std::vector<Triangle> faces;
    std::multimap<int, int> vertex_triangle_associtations;

public:
    Mesh(int mid = 1, int matID = 1) : id(mid), materialID(matID), shmode(ShadingMode::Flat) {}
    Mesh(const Mesh& m) = delete;
    Mesh(Mesh&& m) = default;

    void AddFace(Triangle& face) {
        faces.push_back(std::move(face));
    }

    void AssociateV2T();
    void InsertVT(Triangle face);
    void SetNormal(Vertex& vert);

    void ShadingMode(ShadingMode mode) { shmode = mode; }

    boost::optional<HitInfo> Hit(const Ray &ray) const;
    bool FastHit(const Ray &ray) const;

    int ID() const { return id; }
};

std::vector<Mesh> LoadMeshes(tinyxml2::XMLElement *elem);