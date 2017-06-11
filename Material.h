////
//// Created by Göksu Güvendiren on 04/03/2017.
////
//
//#pragma once
//
//#include <vector>
//#include <glm/vec3.hpp>
//#include "tinyxml/tinyxml2.h"
//
//class Material
//{
//    glm::vec3 diffuse;
//    glm::vec3 specular;
//    glm::vec3 ambient;
//
//    glm::vec3 mirrorReflectance;
//    glm::vec3 transparency;
//
//    float refractionIndex;
//    float phongExponent;
//    float roughness;
//
//    bool isMirror = false;
//    bool isTransparent = false;
//
//    int id;
//    int brdf_id = -1;
//
//    bool isLight = false;
//    
//    Material() = default;
//public:
//    Material(int i, glm::vec3 a, glm::vec3 d, glm::vec3 s,
//             glm::vec3 m, glm::vec3 t, float r = 1.0f, float p = 1, float rough = 0.f,
//             bool ism = false, bool ist = false, int bi = -1) : diffuse(d), specular(s), ambient(a),
//                                                   mirrorReflectance(m), transparency(t),
//                                                   refractionIndex(r), phongExponent(p),
//                                                   roughness(rough), isMirror(ism),
//                                                   isTransparent(ist), id(i), brdf_id(bi) {}
//    
//
//    static Material MakeLightMat(int i, const glm::vec3& col)
//    {
//        Material m;
//        m.id = i;
//        m.isLight = true;
//        m.diffuse = col;
//        return m;
//    }
//
//    glm::vec3 Diffuse() const { return diffuse; }
//    glm::vec3 Specular() const { return specular; }
//    glm::vec3 Ambient() const { return ambient; }
//    glm::vec3 Mirror() const { return mirrorReflectance; }
//    glm::vec3 Transparency() const { return transparency; }
//    
//    glm::vec3 GetLight() const { return diffuse; }
//
//    bool IsMirror() const { return isMirror; }
//    bool IsTransparent() const { return isTransparent; }
//    bool IsLight() const { return isLight; }
//
//    float PhongExp() const { return phongExponent; }
//    float Roughness() const { return roughness; }
//    float RefractionIndex() const { return refractionIndex; }
//
//    auto ID() const { return id; }
//    int BRDF_ID() const { return brdf_id; }
//};
//
//std::vector<Material> LoadMaterials(tinyxml2::XMLElement *elem);
