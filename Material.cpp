//
// Created by Göksu Güvendiren on 04/03/2017.
//

#include "Material.h"
#include <sstream>

inline glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

std::vector<Material> CreateMaterials(tinyxml2::XMLElement* elem)
{
    std::vector<Material> mats;
    for (auto child = elem->FirstChildElement("Material"); child != NULL; child = child->NextSiblingElement()){
        int id;
        child->QueryIntAttribute("id", &id);

        auto ambient  = GetElem(child->FirstChildElement("AmbientReflectance"));
        auto diffuse  = GetElem(child->FirstChildElement("DiffuseReflectance"));
        auto specular = GetElem(child->FirstChildElement("SpecularReflectance"));
        float phongEx = child->FirstChildElement("PhongExponent")->FloatText(1);

        mats.push_back(Material(id, ambient, diffuse, specular, phongEx));
    }

    return mats;
}