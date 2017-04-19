//
// Created by Göksu Güvendiren on 17/04/2017.
//

#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <sstream>
#include <iostream>

static glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

std::vector<std::unique_ptr<Light>> LoadLights(tinyxml2::XMLElement *elem)
{
    std::vector<std::unique_ptr<Light>> lights;

    for (auto child = elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (child->Name() == std::string("PointLight")){
            std::cerr << "hi point" << '\n';
            int id;
            child->QueryIntAttribute("id", &id);

            auto position  = GetElem(child->FirstChildElement("Position"));
            auto intensity = GetElem(child->FirstChildElement("Intensity"));

            lights.push_back(std::make_unique<PointLight>(id, position, intensity));
        }
        else if (child->Name() == std::string("SpotLight")){
            std::cerr << "hi spot" << '\n';

            int id;
            child->QueryIntAttribute("id", &id);

            auto position  = GetElem(child->FirstChildElement("Position"));
            auto intensity = GetElem(child->FirstChildElement("Intensity"));
            auto direction = GetElem(child->FirstChildElement("Direction"));

            auto coverAng = child->FirstChildElement("CoverageAngle")->FloatText();
            auto fallAng  = child->FirstChildElement("FalloffAngle")->FloatText();

            lights.push_back(std::make_unique<SpotLight>(id, position, intensity, direction, coverAng, fallAng));
        }
    }

    return lights;
}