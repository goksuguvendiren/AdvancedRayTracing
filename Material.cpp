////
//// Created by Göksu Güvendiren on 04/03/2017.
////
//
//#include "Material.h"
//#include <sstream>
//
//static glm::vec3 GetElem(tinyxml2::XMLElement* element)
//{
//    glm::vec3 color;
//
//    std::istringstream ss {element->GetText()};
//    ss >> color.r;
//    ss >> color.g;
//    ss >> color.b;
//
//    return color;
//}
//
//std::vector<Material> LoadMaterials(tinyxml2::XMLElement *elem)
//{
//    std::vector<Material> mats;
//    for (auto child = elem->FirstChildElement("Material"); child != NULL; child = child->NextSiblingElement()){
//        int id;
//        child->QueryIntAttribute("id", &id);
//        
//        int brdf_id = -1;
//        if (child->QueryIntAttribute("BRDF", &brdf_id));
//
//        auto  ambient  = GetElem(child->FirstChildElement("AmbientReflectance"));
//        auto  diffuse  = GetElem(child->FirstChildElement("DiffuseReflectance"));
//        auto  specular = GetElem(child->FirstChildElement("SpecularReflectance"));
//
//        tinyxml2::XMLElement* tmp;
//        glm::vec3  mirror = {0, 0, 0}, transparency = {0, 0, 0};
//        float refIndex = 1;
//        float phongEx = 0;
//
//        bool ismirror = false;
//        bool istransparent = false;
//
//
//        if ((tmp = child->FirstChildElement("PhongExponent")))
//            phongEx  = tmp->FloatText(1);
//
//        if ((tmp = child->FirstChildElement("MirrorReflectance"))) {
//            mirror = GetElem(tmp);
//            if (mirror != glm::vec3{0, 0, 0}) ismirror = true;
//        }
//
//        if ((tmp = child->FirstChildElement("Transparency"))) {
//            transparency = GetElem(tmp);
//            if (transparency != glm::vec3{0, 0, 0}) istransparent = true;
//        }
//
//        float roughness = 0;
//        if ((tmp = child->FirstChildElement("Roughness"))) {
//            roughness = tmp->FloatText(0);
//        }
//
//        if ((tmp = child->FirstChildElement("RefractionIndex")))
//            refIndex = tmp->FloatText(1);
//
//        mats.push_back(Material(id, ambient, diffuse, specular, mirror, transparency,
//                                refIndex, phongEx, roughness, ismirror, istransparent, brdf_id));
//    }
//
//    return mats;
//}
