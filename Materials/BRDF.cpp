#include "../HitInfo.h"
#include "BRDF.hpp"
#include "PhongBRDF.hpp"
#include "BlinnPhongBRDF.hpp"
#include "PhongBRDF_Mdf.hpp"
#include "BlinnPhongBRDF_Mdf.hpp"

std::vector<std::unique_ptr<BRDF>> LoadBRDFs(tinyxml2::XMLElement *elem)
{
    std::vector<std::unique_ptr<BRDF>> mats;
    
    for (auto child = elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (child->Name() == std::string("OriginalPhong"))
        {
            int id;
            child->QueryIntAttribute("id", &id);
            
            bool normalized = false;
            if (child->QueryBoolAttribute("normalized", &normalized));
            
            auto exponent = child->FirstChildElement("Exponent")->FloatText(0.f);
            mats.push_back(std::make_unique<Phong>(id, exponent, normalized));
        }
    }
    
    for (auto child = elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (child->Name() == std::string("OriginalBlinnPhong"))
        {
            int id;
            child->QueryIntAttribute("id", &id);
            
            bool normalized = false;
            if (child->QueryBoolAttribute("normalized", &normalized));

            auto exponent = child->FirstChildElement("Exponent")->FloatText(0.f);
            mats.push_back(std::make_unique<BlinnPhong>(id, exponent, normalized));
        }
    }
    
    for (auto child = elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (child->Name() == std::string("ModifiedPhong"))
        {
            int id;
            child->QueryIntAttribute("id", &id);
            
            bool normalized = false;
            if (child->QueryBoolAttribute("normalized", &normalized));

            auto exponent = child->FirstChildElement("Exponent")->FloatText(0.f);
            mats.push_back(std::make_unique<PhongModified>(id, exponent, normalized));
        }
    }
    for (auto child = elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (child->Name() == std::string("ModifiedBlinnPhong"))
        {
            int id;
            child->QueryIntAttribute("id", &id);
            
            bool normalized = false;
            if (child->QueryBoolAttribute("normalized", &normalized));

            auto exponent = child->FirstChildElement("Exponent")->FloatText(0.f);
            mats.push_back(std::make_unique<BlinnPhongModified>(id, exponent, normalized));
        }
    }
    
    return mats;
}
