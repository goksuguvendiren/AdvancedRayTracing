//
// Created by Göksu Güvendiren on 15/03/2017.
//

#include <sstream>
#include <iostream>
#include "Transformation.h"
#include "Material.h"
#include "glm/gtc/matrix_transform.hpp"

inline glm::vec3 GetElem(std::istringstream& ss)
{
    glm::vec3 color;
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

inline float GetAngle(std::istringstream& ss)
{
    float angle;
    ss >> angle;

    return angle;
}

std::map<std::string, glm::mat4> CreateTransformations(tinyxml2::XMLElement* elem)
{
    std::map<std::string, glm::mat4> trs;

    auto translations = std::move(CreateTranslations(elem));
    for (auto translation : translations){
        trs.insert(translation);
    }

    auto rotations = std::move(CreateRotations(elem));
    for (auto rotation : rotations){
        trs.insert(rotation);
    }

    auto scalings = std::move(CreateScalings(elem));
    for (auto scaling : scalings){
        trs.insert(scaling);
    }

    return trs;
}

std::map<std::string, glm::mat4> CreateTranslations(tinyxml2::XMLElement* elem)
{
    std::map<std::string, glm::mat4> trs;
    for (auto child = elem->FirstChildElement("Translation"); child != NULL; child = child->NextSiblingElement()){
        int id;
        child->QueryIntAttribute("id", &id);

        std::istringstream ss {child->GetText()};
        auto details = GetElem(ss);
        auto View = glm::translate(glm::mat4(1.), details);

        auto sth = "t" + std::to_string(id);
        trs.insert(std::make_pair(sth, View));
    }

    return trs;
}


std::map<std::string, glm::mat4> CreateRotations(tinyxml2::XMLElement* elem)
{
    std::map<std::string, glm::mat4> rts;
    for (auto child = elem->FirstChildElement("Rotation"); child != NULL; child = child->NextSiblingElement()){
        int id;
        child->QueryIntAttribute("id", &id);

        std::istringstream ss {child->GetText()};

        auto angle   = GetAngle(ss);
        auto details = GetElem(ss);
        auto matrix  = glm::rotate(glm::mat4(1.), angle, details);

        auto sth = "r" + std::to_string(id);
        rts.insert(std::make_pair(sth, matrix));
    }

    return rts;
}

std::map<std::string, glm::mat4> CreateScalings(tinyxml2::XMLElement* elem)
{
    std::map<std::string, glm::mat4> scls;
    for (auto child = elem->FirstChildElement("Scaling"); child != NULL; child = child->NextSiblingElement()){
        int id;
        child->QueryIntAttribute("id", &id);

        std::istringstream ss {child->GetText()};
        auto details = GetElem(ss);
        auto matrix = glm::scale(glm::mat4(1.), details);

        auto sth = "s" + std::to_string(id);
        scls.insert(std::make_pair(sth, matrix));
    }

    return scls;
}
