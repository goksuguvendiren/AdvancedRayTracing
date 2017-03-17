//
// Created by Göksu Güvendiren on 06/03/2017.
//

#include <vector>
#include <sstream>
#include "Vertex.h"
#include "tinyxml/tinyxml2.h"

glm::vec3 GetVertex(std::istringstream& stream)
{
    Vertex vert;

    float datax;
    float datay;
    float dataz;

    stream >> datax;
    stream >> datay;
    stream >> dataz;

    return {datax, datay, dataz};
}

std::vector<glm::vec3> LoadVertexData(tinyxml2::XMLElement *elem)
{
    std::istringstream stream { elem->GetText() };

    std::vector<glm::vec3> verts;
    while(stream){
        verts.push_back(GetVertex(stream));
    }

    return verts;
}