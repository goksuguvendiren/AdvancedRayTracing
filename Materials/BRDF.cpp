#include "../HitInfo.h"
#include "BRDF.hpp"
#include "PhongBRDF.hpp"
#include "BlinnPhongBRDF.hpp"

std::vector<std::unique_ptr<BRDF>> LoadBRDFs(tinyxml2::XMLElement *elem)
{
    std::vector<std::unique_ptr<BRDF>> mats;
    
    for (auto child = elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (child->Name() == std::string("OriginalPhong"))
        {
            int id;
            child->QueryIntAttribute("id", &id);
            
            auto exponent = child->FirstChildElement("Exponent")->FloatText(0.f);
            mats.push_back(std::make_unique<Phong>(id, exponent));
        }
    }
    
    for (auto child = elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if (child->Name() == std::string("OriginalBlinnPhong"))
        {
            int id;
            child->QueryIntAttribute("id", &id);
            
            auto exponent = child->FirstChildElement("Exponent")->FloatText(0.f);
            mats.push_back(std::make_unique<BlinnPhong>(id, exponent));
        }
    }
    
    

    
    return mats;
}
