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

    for (int i = 0; i < imagePlane.NX(); i++){
        for (int j = 0; j < imagePlane.NY(); j++){
            auto pixLocation = GetPixelLocation(i, j);
            auto ray = Ray(position, pixLocation);

            for (auto sphere : scene.Spheres()) {
                if (sphere.Hit(ray)){
                    image.at(i, j) = Color(255, 255, 255);
                }
                else {
                    image.at(i, j) = Color(0, 0, 0);
                }
            }

            for (auto triangle : scene.Triangles()){
                if (triangle.Hit(ray)){
                    image.at(i, j) = Color(255, 255, 255);
                }
                else {
                    image.at(i, j) = Color(0, 0, 0);
                }
            }
        }
    }

    return image;
}

glm::vec3 Camera::GetPixelLocation(int i, int j) const
{
        return planePosition + (i + 0.5f) * imagePlane.PixelWidth() * right -
                           (j + 0.5f) * imagePlane.PixelHeight() * up;
}