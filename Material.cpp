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

std::vector<Material> LoadMaterials(tinyxml2::XMLElement *elem)
{
    std::vector<Material> mats;
    for (auto child = elem->FirstChildElement("Material"); child != NULL; child = child->NextSiblingElement()){
        int id;
        child->QueryIntAttribute("id", &id);

        auto  ambient  = GetElem(child->FirstChildElement("AmbientReflectance"));
        auto  diffuse  = GetElem(child->FirstChildElement("DiffuseReflectance"));
        auto  specular = GetElem(child->FirstChildElement("SpecularReflectance"));
        float phongEx  = child->FirstChildElement("PhongExponent")->FloatText(1);

        tinyxml2::XMLElement* tmp;
        glm::vec3  mirror = {0, 0, 0}, transparency = {0, 0, 0};
        float refIndex = 1;

        if ((tmp = child->FirstChildElement("MirrorReflectance")))
            mirror = GetElem(tmp);

        if ((tmp = child->FirstChildElement("Transparency")))
            transparency = GetElem(tmp);

        if ((tmp = child->FirstChildElement("RefractionIndex")))
            refIndex = tmp->FloatText(1);

        mats.push_back(Material(id, ambient, diffuse, specular, mirror, transparency, refIndex, phongEx));
    }

    return mats;
}