//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <iostream>
#include "Image.h"

void Image::Print()
{
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            std::cout << at(i, j).R() << ", ";
        }
        std::cout << '\n';
    }
}