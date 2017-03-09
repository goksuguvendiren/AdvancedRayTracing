#include <iostream>

#include "Camera.h"
#include "Scene.h"
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>

Scene scene;

int main()
{
    std::string sceneName = "simple";
    std::cerr << "Started loading the scene...\n";
    scene.CreateScene("/Users/goksu/Documents/AdvancedRayTracer/inputs/" + sceneName + ".xml");
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
                im.at<cv::Vec3f>(i, j)[0] = image.at(i, j).b * 255;
                im.at<cv::Vec3f>(i, j)[1] = image.at(i, j).g * 255;
                im.at<cv::Vec3f>(i, j)[2] = image.at(i, j).r * 255;
            }
        }

        cv::imshow("output image", cv::Mat(im));
        cv::waitKey(0);
        cv::imwrite("/Users/goksu/Documents/AdvancedRayTracer/outputs/bunny" + sceneName + ".png", im);
    }

    return 0;
}