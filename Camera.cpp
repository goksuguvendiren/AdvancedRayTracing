//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <iostream>
#include <sstream>
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include "Shapes/Triangle.h"
#include "Lights/PointLight.h"
#include <random>
#include <functional>
#include "PerlinNoise.hpp"

std::vector<int> samples;
std::vector<int> grsamples;

glm::vec3 Camera::CalculateReflectance(const HitInfo& hit, int recDepth) const
{
    if (hit.Texture() && hit.Texture()->DecalMode() == DecalMode::Replace_All)
    {
        glm::vec2 texCoords = hit.GetUV();
        return hit.Texture()->GetColor(texCoords) * 255.f;
    }

    // Ambient shading :
    auto ambient = hit.Material().Ambient() * scene.AmbientLight();

    if (hit.Material().IsMirror() && recDepth < scene.MaxRecursionDepth()) {
        auto reflectedColor = CalculateMirror(hit, recDepth);
        ambient += reflectedColor;
    }

    if (hit.Material().IsTransparent() && recDepth < scene.MaxRecursionDepth()) {
        auto trColor = CalculateTransparency(hit, recDepth);
        if (trColor != glm::vec3{0, 0, 0})
            ambient += trColor;
    }

    for (auto& light : scene.Lights()){
        auto direction = light->Direction(hit.Position());
        Ray shadowRay(hit.Position() + (scene.ShadowRayEpsilon() * glm::normalize(direction)),
                      glm::normalize(direction));

        boost::optional<HitInfo> sh;
        if ((sh = scene.Hit(shadowRay))){
            if (sh->Parameter() < glm::length(direction))
                continue;
        }
        
//        boost::optional<float> sh;
//        if ((sh = scene.ShadowHit(shadowRay))){
//            if (*sh < glm::length(direction))
//                continue;
//        }

        auto intensity = light->Intensity(direction);

        auto diffuse_color = hit.Material().Diffuse();
        if(hit.Texture())
        {
            if (hit.Texture()->IsPerlin()) {
                auto noise = hit.Texture()->Perlin().Sample(hit.Position());
                diffuse_color = hit.Texture()->BlendColor(diffuse_color, {noise, noise, noise});
            }
            else {
                glm::vec2 texCoords = hit.GetUV();
                glm::vec3 tex_color = hit.Texture()->GetColor(texCoords);
                diffuse_color = hit.Texture()->BlendColor(diffuse_color, tex_color);
            }
        }

        // Diffuse shading :
        auto theta = std::max(0.f, glm::dot(glm::normalize(hit.Normal()), glm::normalize(direction)));
        ambient += (theta * diffuse_color * intensity);

        // Specular shading :
        auto half = glm::normalize(glm::normalize(direction) + glm::normalize(hit.HitRay().Origin() - hit.Position()));
        ambient  += intensity *
                    hit.Material().Specular() *
                    std::pow(std::max(glm::dot(half, hit.Normal()), 0.0f), hit.Material().PhongExp());
    }

    return ambient;
}

glm::vec3 Camera::GetCameraPosition() const
{
    if (apertureSize == 0)
    {
        return position;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(samples.begin(), samples.end(), g);

    auto index = samples.back();
    samples.pop_back();

    int indH = index / divCount;
    int indW = index % divCount;

    float cellWidth  = apertureSize / float(divCount);
    float cellHeight = apertureSize / float(divCount);

    auto oneRight = cellWidth * right;
    auto oneDown  = -cellHeight * up;

    // For sampling in the camera, we now think of our camera not as a pinhole, but a camera with a lens.
    // Number of samples are the same in camera, pixel and arealight.

    auto camLocation = position - ((imagePlane.PixelWidth() / 2.f) * right);
    camLocation -= ((imagePlane.PixelHeight() / 2.f) * up);
    camLocation += cellWidth / 2.f;
    camLocation += cellHeight / 2.f;

    return camLocation + (float(indW) * oneRight + float(indH) * oneDown);
}

glm::vec3 Camera::RenderPixel(const glm::vec3& pixelcenter) const
{
    glm::vec3 pixelColor = {0, 0, 0};

    samples = grsamples;

    float cellWidth  = imagePlane.PixelWidth()  / float(divCount);
    float cellHeight = imagePlane.PixelHeight() / float(divCount);

    auto oneRight = cellWidth * right;
    auto oneDown  = -cellHeight * up;

    // label the first pixel, by taking the center of the pixel to the top left
    glm::vec3 pixelBeginning = pixelcenter - ((imagePlane.PixelWidth() / 2.f) * right);
    pixelBeginning -= ((imagePlane.PixelHeight() / 2.f) * up);
    // then add half a cell size to locate the center of the cell.
    pixelBeginning += cellWidth / 2.f;
    pixelBeginning += cellHeight / 2.f;

    for (int i = 0; i < sampleCount; i++) {
        //calculate pixel location
        int indH = i / divCount;
        int indW = i % divCount;

        glm::vec3 pixelLocation = pixelBeginning + (float(indW) * oneRight + float(indH) * oneDown);
        glm::vec3 cameraLocation = GetCameraPosition();

        auto ray = Ray(cameraLocation, glm::normalize(pixelLocation - cameraLocation));

        boost::optional<HitInfo> hit  = scene.Hit(ray);

        if (hit)
        {
            pixelColor += CalculateReflectance(*hit, 0);
        }
        else
        {
            pixelColor += scene.BackgroundColor();
        }
    }

    return pixelColor / float(sampleCount);
}

Image Camera::Render() const
{
    Image image(imagePlane.NX(), imagePlane.NY());

    grsamples = samples;

    std::cerr << "Rendering with " << sampleCount << " samples for pixels." << '\n';

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
            image.at(i, j) = RenderPixel(rowPixLocation);
        }

        auto progress = i / (float)imagePlane.NY();

        auto UpdateProgress = [](float progress)
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
        };

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

    glm::vec3 position = GetElem(element->FirstChildElement("Position"));
    glm::vec3 gaze = GetElem(element->FirstChildElement("Gaze"));
    glm::vec3 up = GetElem(element->FirstChildElement("Up"));

    int sampleCount = element->FirstChildElement("NumSamples")->IntText(1);

    int focalDistance = 1;
    tinyxml2::XMLElement* elem;
    if ((elem = element->FirstChildElement("FocusDistance"))){
        focalDistance = elem->IntText(1);
    }

    float apertureSize = 0;
    if ((elem = element->FirstChildElement("ApertureSize"))){
        apertureSize = elem->FloatText(0);
    }

    ImagePlane plane = CreatePlane(element, focalDistance);

    std::string name = element->FirstChildElement("ImageName")->GetText();

    return Camera(plane, id, position, gaze, up, name, sampleCount, focalDistance, apertureSize);
}

auto getT(const glm::vec3& v)
{
    auto vec = glm::abs(v);
    if (vec.x < vec.y && vec.x < vec.z){
        return glm::normalize(glm::vec3{1, v.y, v.z});
    }
    if (vec.y < vec.x && vec.y < vec.z){
        return glm::normalize(glm::vec3{v.x, 1, v.z});
    }

    return glm::normalize(glm::vec3{v.x, v.y, 1});
}

std::mt19937 mirrorseed;


glm::vec3 Camera::CalculateMirror(const HitInfo& hit, int recDepth) const
{
    if (hit.Material().Roughness() < 0.0001)
    {
        auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
        float costheta = glm::dot(v, hit.Normal());
        auto dir = glm::normalize(2.f * costheta * hit.Normal() - v);

        Ray reflectionRay(hit.Position() + (scene.ShadowRayEpsilon() * dir),
                          dir);

        boost::optional<HitInfo> refHit = scene.Hit(reflectionRay);

        glm::vec3 reflectedColor = {0, 0, 0};
        if (refHit)
            reflectedColor += hit.Material().Mirror() * CalculateReflectance(*refHit, recDepth + 1);

        return reflectedColor;
    }
    else
    {
        auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
        float costheta = glm::dot(v, hit.Normal());
        auto dir = glm::normalize(2.f * costheta * hit.Normal() - v);

        glm::vec3 t = glm::normalize(getT(dir));

//        assert(glm::dot(dir, t) < 0.001 && glm::dot(dir, t) > 0.99);

        auto m = glm::normalize(glm::cross(dir, t));
//        std::cerr << glm::dot(dir, m) << '\n';

        auto u = glm::normalize(glm::cross(m, dir));
//        std::cerr << glm::dot(dir, u) << '\n';

        auto generateRandomFloat = []()
        {
            std::uniform_real_distribution<float> asd(0, 1);
            return asd(mirrorseed) - 0.5f;
        };

        m = hit.Material().Roughness() * generateRandomFloat() * m;
        u = hit.Material().Roughness() * generateRandomFloat() * u;

        auto newdir = glm::normalize(dir + m + u);

        Ray reflectionRay(hit.Position() + (scene.ShadowRayEpsilon() * newdir), newdir);

        boost::optional<HitInfo> refHit = scene.Hit(reflectionRay);

        glm::vec3 reflectedColor = {0, 0, 0};
        if (refHit)
            reflectedColor += hit.Material().Mirror() * CalculateReflectance(*refHit, recDepth + 1);

        return reflectedColor;
    }
}

glm::vec3 Camera::CalculateTransparency(const HitInfo &hit, int recDepth) const
{
    bool entering = (glm::dot(hit.Normal(), hit.HitRay().Direction()) < 0);
    glm::vec3 t;

    glm::vec3 normal;
    float ratio;
    glm::vec3 refractedColor = {0, 0, 0};

    if (entering) {
        normal = hit.Normal();
        auto n1 = 1.0f;
        auto n2 = hit.Material().RefractionIndex();

        ratio = n1 / n2;
        float cost = glm::dot(-hit.HitRay().Direction(), normal);
        float delta = 1 - std::pow(ratio, 2.0f) * (1 - cost * cost);

        float c2 = std::sqrt(delta);
        float c1 = cost;

        t = ratio * hit.HitRay().Direction() + (ratio * c1 - c2) * normal;
        t = glm::normalize(t);

        //
        auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
        float costheta = glm::dot(v, normal);
        auto reflDir = glm::normalize(2.f * costheta * normal - v);

        Ray reflectingRay(hit.Position() + (scene.ShadowRayEpsilon() * reflDir), reflDir);

        boost::optional<HitInfo> reflectionHit = scene.Hit(reflectingRay);

        glm::vec3 reflectioncolor = {0, 0, 0};
        if (reflectionHit)
            reflectioncolor = CalculateReflectance(*reflectionHit, recDepth + 1);

        float r0 = std::pow(((n1 - n2) / (n1 + n2)), 2);
        float rSchlick = r0 + ((1.0f - r0) * std::pow(1 - costheta, 5.0f));

        reflectioncolor *= rSchlick;

        Ray refractionRay(hit.Position() + (scene.ShadowRayEpsilon() * t), t);
        boost::optional<HitInfo> refHit = scene.Hit(refractionRay);

        if (refHit) {
            auto calcColor =  CalculateReflectance(*refHit, recDepth + 1);
            float r = calcColor.r * (std::exp(std::log(hit.Material().Transparency().r) * refHit->Parameter()));
            float g = calcColor.g * (std::exp(std::log(hit.Material().Transparency().g) * refHit->Parameter()));
            float b = calcColor.b * (std::exp(std::log(hit.Material().Transparency().b) * refHit->Parameter()));

            refractedColor += (1 - rSchlick) * glm::vec3{r, g, b};
        }

        refractedColor += reflectioncolor;
    }
    else
    { //exiting
        normal = -hit.Normal();

        float n1 = hit.Material().RefractionIndex();
        float n2 = 1.0f;
        ratio = n1 / n2;

        float costheta = glm::dot(-hit.HitRay().Direction(), normal);
        float delta = 1 - std::pow(ratio, 2.0f) * (1 - costheta * costheta);

        if (delta < 0)
        {
            auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
            float costheta = glm::dot(v, normal);
            auto dir = glm::normalize(2.f * costheta * normal - v);

            Ray reflectionRay(hit.Position() + (scene.ShadowRayEpsilon() * dir),
                              dir);

            boost::optional<HitInfo> refHit = scene.Hit(reflectionRay);

            glm::vec3 reflectedColor = {0, 0, 0};
            if (refHit)
            {
                reflectedColor += CalculateReflectance(*refHit, recDepth + 1);

                float r = reflectedColor.r * (std::exp(std::log(hit.Material().Transparency().r) * refHit->Parameter()));
                float g = reflectedColor.g * (std::exp(std::log(hit.Material().Transparency().g) * refHit->Parameter()));
                float b = reflectedColor.b * (std::exp(std::log(hit.Material().Transparency().b) * refHit->Parameter()));

                reflectedColor = {r, g, b};

            }

            return reflectedColor;
        }

        float c2 = std::sqrt(delta);
        float c1 = costheta;

        t = ratio * hit.HitRay().Direction() + (ratio * c1 - c2) * normal;
        t = glm::normalize(t);


        auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
        float cost = glm::dot(v, normal);
        auto reflDir = glm::normalize(2.f * cost * normal - v);

        Ray reflectingRay(hit.Position() + (scene.ShadowRayEpsilon() * reflDir), reflDir);

        boost::optional<HitInfo> reflectionHit = scene.Hit(reflectingRay);

        glm::vec3 reflectioncolor = {0, 0, 0};
        if (reflectionHit)
            reflectioncolor = CalculateReflectance(*reflectionHit, recDepth + 1);

        float r0 = std::pow(((n1 - n2) / (n1 + n2)), 2);
        float rSchlick = r0 + ((1.0f - r0) * std::pow(1 - cost, 5.0f));

        reflectioncolor *= rSchlick;


        Ray refractionRay(hit.Position() + (scene.ShadowRayEpsilon() * t), t);
        boost::optional<HitInfo> refHit = scene.Hit(refractionRay);

        if (refHit) {
            auto calcColor = CalculateReflectance(*refHit, recDepth + 1);

            refractedColor += (1 - rSchlick) * calcColor;
        }

        refractedColor += reflectioncolor;
    }

    return refractedColor;
}
