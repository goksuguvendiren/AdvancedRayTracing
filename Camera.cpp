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

    std::cout << "PlanePosition is : " << planePosition.x << ", " << planePosition.y << ", " << planePosition.z << '\n';
    for (int i = 0; i < imagePlane.NX(); i++){
        for (int j = 0; j < imagePlane.NY(); j++){
            auto pixLocation = GetPixelLocation(i, j);
            auto ray = Ray(position, pixLocation);
            std::cout << "(" << ray.Origin().x << ", " << ray.Origin().y << ", " << ray.Origin().z << "), " <<
                         "(" << ray.Direction().x << ", " << ray.Direction().y << ", " << ray.Direction().z << ")\n";

            for (auto sphere : scene.Spheres()) {
                if (sphere.Hit(ray)){
//                    std::cerr << "hit" << '\n';
                    image.at(i, j) = Color(255, 255, 255);
                }
                else {
                    image.at(i, j) = Color(0, 0, 0);
                }
            }
        }
    }

    image.Print();
    return image;
}

glm::vec3 Camera::GetPixelLocation(int i, int j) const
{
        return planePosition + (i + 0.5f) * imagePlane.PixelWidth() * right -
                           (j + 0.5f) * imagePlane.PixelHeight() * up;
}