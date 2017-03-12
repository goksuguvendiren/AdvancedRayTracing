//
// Created by Göksu Güvendiren on 06/03/2017.
//

#pragma once

#include <vector>
#include "Triangle.h"

class Mesh
{
    int id;
    int materialID;
    std::vector<Triangle> faces;
public:
    Mesh(int mid = 1, int matID = 1) : id(mid), materialID(matID) {}
    Mesh(const Mesh& m) = delete;
    Mesh(Mesh&& m) = default;

    void AddFace(int a, int b, int c) { faces.push_back( Triangle{a, b, c} ); }
    void AddFace(const Triangle& face) { faces.push_back(face); }
    std::pair<bool, HitInfo> Hit(const Ray &ray) const;
    bool BoolHit(const Ray &ray) const;
};

std::vector<Mesh> CreateMeshes(tinyxml2::XMLElement* elem);