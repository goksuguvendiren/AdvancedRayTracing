#include <iostream>

#include "Camera.h"
#include "Scene.h"
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>

Scene scene;

int main(int argc, char** argv)
{
    std::string sceneName = "simple_transform";
    if (argc == 2){
        sceneName = argv[1];
    }
    std::cerr << "Started loading the scene " << sceneName << "...\n";
    scene.CreateScene("/Users/goksu/Documents/AdvancedRayTracer/hw2_inputs/" + sceneName + ".xml");
    std::cerr << "Finished loading...\n";

    std::vector<Image> images;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    std::cerr << "Started rendering the scene...\n";
    for (auto& cam : scene.Cameras()){
        images.push_back(cam.Render());
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cerr << "Rendering took "
              << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
              << "s.\n";

    for (const auto& image : images){
        cv::Mat im = cv::Mat(image.Height(), image.Width(), CV_32FC3);

        for (int i = 0; i < image.Height(); i++){
            for (int j = 0; j < image.Width(); j++){
                // ATTENTION : OpenCV expects BGR color space
                im.at<cv::Vec3f>(i, j)[0] = image.at(i, j).b;
                im.at<cv::Vec3f>(i, j)[1] = image.at(i, j).g;
                im.at<cv::Vec3f>(i, j)[2] = image.at(i, j).r;
            }
        }

        cv::imwrite("/Users/goksu/Documents/AdvancedRayTracer/hw2_outputs/" + sceneName + ".png", im);
    }

    return 0;
}