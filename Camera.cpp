//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <iostream>
#include <sstream>
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include "Triangle.h"
#include "BoundingVolume.h"
#include "LightSource.h"

glm::vec3 Camera::CalculateMirror(const HitInfo& hit, int recDepth) const
{
    auto dir = glm::normalize(2.f * glm::dot(Position() - hit.Position(), hit.Normal()) * hit.Normal() -
                              Position() - hit.Position());

    Ray reflectionRay(hit.Position() + (scene.ShadowRayEpsilon() * dir),
                      dir);

    boost::optional<HitInfo> refHit = scene.BoundingBox().Hit(reflectionRay);

    glm::vec3 reflectedColor;
    if (refHit)
        reflectedColor = hit.Material().Mirror() * CalculateReflectance(*refHit, recDepth + 1);
    else
        reflectedColor = scene.BackgroundColor();

    return reflectedColor;
}

glm::vec3 Camera::CalculateReflectance(const HitInfo& hit, int recDepth) const
{
    // Ambient shading :
    auto ambient = hit.Material().Ambient() * scene.AmbientLight();

    if (hit.Material().IsMirror() && recDepth < scene.MaxRecursionDepth()) {
        auto reflectedColor = CalculateMirror(hit, recDepth);
        ambient += reflectedColor;
    }

    for (auto& light : scene.Lights()){
        auto direction = glm::normalize(light.Position() - hit.Position());
        Ray shadowRay(hit.Position() + (scene.ShadowRayEpsilon() * direction),
                      direction);

        boost::optional<HitInfo> sh;
        if (sh = scene.BoundingBox().Hit(shadowRay)){
//            std::cerr << sh->Parameter() << '\n';
            assert(sh->Parameter() > 0);
            continue;
        }

        glm::vec3 pointToLight = light.Position() - hit.Position();
        auto intensity = light.Intensity(pointToLight);

        pointToLight = glm::normalize(pointToLight);

        // Diffuse shading :
        auto theta = glm::dot(glm::normalize(hit.Normal()), pointToLight);
        ambient += (theta * hit.Material().Diffuse() * intensity);

        // Specular shading :
        auto half = glm::normalize(pointToLight + glm::normalize(Position() - hit.Position()));
        ambient  += intensity *
                    hit.Material().Specular() *
                    std::pow(std::max(glm::dot(half, hit.Normal()), 0.0f), hit.Material().PhongExp());
    }

    return ambient;
}

void UpdateProgress(float progress)
{
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

Image Camera::Render() const
{
    Image image(imagePlane.NX(), imagePlane.NY());

    auto oneRight = imagePlane.PixelWidth() * right;
    auto oneDown  = -imagePlane.PixelHeight() * up;

    auto pixLocation = PlanePosition();
    pixLocation -= oneRight * 0.5f;
    pixLocation -= oneDown * 0.5f;

    auto rowPixLocation = pixLocation;
    auto rowBeginning   = pixLocation;

    for (int i = 0; i < imagePlane.NY(); i++){          // ny = height
        rowBeginning += oneDown;
        rowPixLocation = rowBeginning;
        for (int j = 0; j < imagePlane.NX(); j++){      // nx = width
            rowPixLocation += oneRight;
            auto ray = Ray(position, rowPixLocation - position);

            boost::optional<HitInfo> hit  = scene.BoundingBox().Hit(ray);

            if (hit)
                image.at(i, j) = CalculateReflectance(*hit, 0);
//                image.at(i, j) = 255.0f * (hit->Normal() + glm::vec3{1, 1, 1}) / 2.0f; //CalculateReflectance(*ultHit);
            else
                image.at(i, j) = scene.BackgroundColor();
        }

        auto progress = i / (float)imagePlane.NY();
        UpdateProgress(progress);
    }
    std::cout << std::endl;

    return image;
}

glm::vec3 Camera::GetPixelLocation(int i, int j) const
{
        return PlanePosition() - (i + 0.5f) * imagePlane.PixelHeight() * up +
                                 (j + 0.5f) * imagePlane.PixelWidth()  * right;
}

static glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

Camera LoadCamera(tinyxml2::XMLElement *element)
{
    int id;
    if (element->QueryIntAttribute("id", &id) == tinyxml2::XML_NO_ATTRIBUTE){
        std::cerr << "No such attribute as id" << '\n';
        std::abort();
    }

    ImagePlane plane = CreatePlane(element);

    glm::vec3 position = GetElem(element->FirstChildElement("Position"));
    glm::vec3 gaze = GetElem(element->FirstChildElement("Gaze"));
    glm::vec3 up = GetElem(element->FirstChildElement("Up"));

    std::string name = element->FirstChildElement("ImageName")->GetText();

    return Camera(plane, id, position, gaze, up, name);
}