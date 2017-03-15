//
// Created by Göksu Güvendiren on 06/03/2017.
//

#pragma once

#include <vector>
#include "Triangle.h"

class Mesh : public Shape
{
    int id;
    int materialID;
    std::vector<Triangle> faces;
public:
    Mesh(int mid = 1, int matID = 1) : id(mid), materialID(matID) {}
    Mesh(const Mesh& m) = delete;
    Mesh(Mesh&& m) = default;

    void AddFace(int a, int b, int c) { faces.push_back( Triangle{scene.GetVertex(a), scene.GetVertex(a), scene.GetVertex(a)} ); }
    void AddFace(const Triangle& face) { faces.push_back(face); }
    std::pair<bool, HitInfo> Hit(const Ray &ray) const;
    bool FastHit(const Ray &ray) const;

    int ID() const { return id; }
};

std::vector<Mesh> CreateMeshes(tinyxml2::XMLElement* elem);