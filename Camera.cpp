//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <iostream>
#include "Camera.h"
#include "glm/vec3.hpp"
#include "Ray.h"
#include "Scene.h"

extern Scene scene;

Image Camera::Render() const
{
    Image image(imagePlane.NX(), imagePlane.NY());

    for (int i = 0; i < imagePlane.NY(); i++){          // nx = width
        for (int j = 0; j < imagePlane.NX(); j++){      // ny = height
            auto pixLocation = GetPixelLocation(i, j);
            auto ray = Ray(position, pixLocation - position);

            for (auto sphere : scene.Spheres()) {
//                auto val = sphere.Hit(ray);
                std::pair<bool, HitInfo> hit;
                if ((hit = sphere.Hit(ray)).first){
                    image.at(i, j) = hit.second.Material().Ambient();
                }
            }

            for (auto triangle : scene.Triangles()){
                std::pair<bool, HitInfo> hit;
                if ((hit = triangle.Hit(ray)).first){
                    image.at(i, j) = hit.second.Material().Ambient();
                }
            }
        }
    }

    return image;
}

glm::vec3 Camera::GetPixelLocation(int i, int j) const
{
        return PlanePosition() - (i + 0.5f) * imagePlane.PixelHeight() * up +
                                 (j + 0.5f) * imagePlane.PixelWidth()  * right;
}