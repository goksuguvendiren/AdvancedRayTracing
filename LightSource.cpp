//
// Created by Göksu Güvendiren on 09/03/2017.
//

#include "LightSource.h"
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

std::vector<LightSource> LoadLights(tinyxml2::XMLElement *elem)
{
    std::vector<LightSource> lights;
    for (auto child = elem->FirstChildElement("PointLight"); child != NULL; child = child->NextSiblingElement()){
        int id;
        child->QueryIntAttribute("id", &id);

        auto position  = GetElem(child->FirstChildElement("Position"));
        auto intensity = GetElem(child->FirstChildElement("Intensity"));

        lights.push_back(LightSource(id, position, intensity));
    }

    return lights;
}