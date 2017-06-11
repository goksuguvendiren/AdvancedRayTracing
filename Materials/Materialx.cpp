//
// Created by Göksu Güvendiren on 04/03/2017.
//

#include "Materialx.hpp"
#include "ClassicMaterial.hpp"
#include "BlinnPhongBRDF.hpp"
#include "BlinnPhongBRDF_Mdf.hpp"
#include "PhongBRDF.hpp"
#include "PhongBRDF_Mdf.hpp"
#include "Mirror.hpp"
#include <sstream>

static glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

std::vector<Materialx*> LoadMaterials(tinyxml2::XMLElement *elem)
{
    std::vector<Materialx*> mats;
    for (auto child = elem->FirstChildElement("Material"); child != NULL; child = child->NextSiblingElement()){
        int id;
        child->QueryIntAttribute("id", &id);

        int brdf_id = -1;
        if (child->QueryIntAttribute("BRDF", &brdf_id));

        auto  ambient  = GetElem(child->FirstChildElement("AmbientReflectance"));
        auto  diffuse  = GetElem(child->FirstChildElement("DiffuseReflectance"));
        auto  specular = GetElem(child->FirstChildElement("SpecularReflectance"));

        tinyxml2::XMLElement* tmp;
        glm::vec3  mirror = {0, 0, 0}, transparency = {0, 0, 0};
        float refIndex = 1;
        float phongEx = 0;
        
        if ((tmp = child->FirstChildElement("PhongExponent")))
            phongEx  = tmp->FloatText(1);
        
        
        auto mtrl = new ClassicMaterial(id, ambient, diffuse, specular, phongEx, brdf_id);
        
        // Create some classical material with this information
        // Then bind this material with the mirror / transparent / brdf if one exists.
        
        if ((tmp = child->FirstChildElement("MirrorReflectance"))) // then the material should be a mirror
        {
            tinyxml2::XMLElement* tmp_m;
            mirror = GetElem(tmp);
            float roughness = 0;
            if ((tmp_m = child->FirstChildElement("Roughness"))) {
                roughness = tmp_m->FloatText(0);
            }
            
            auto material = new Mirror(id, mirror, roughness);
            mats.push_back(material);
            continue;
        }
        
//        if ((tmp = child->FirstChildElement("Transparency"))) // then the material should be transparent
//        {
//            tinyxml2::XMLElement* tmp_tr;
//            if ((tmp_tr = child->FirstChildElement("RefractionIndex")))
//                refIndex = tmp_tr->FloatText(1);
//
//            transparency = GetElem(tmp);
//            auto material = new Transparent(id, transparency, refIndex);
//            mats.push_back(material);
//            continue;
//        }

        mats.push_back(mtrl);
    }

    return mats;
}
