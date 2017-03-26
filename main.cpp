#include <iostream>

#include "Camera.h"
#include "Scene.h"
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>

Scene scene;
extern std::atomic<std::uint64_t> cnt;

int main(int argc, char** argv)
{
    std::string sceneName = "bunny_flat";

    if (argc == 2){
        sceneName = argv[1];
    }
    std::cerr << "Started loading the scene " << sceneName << "...\n";
    auto start = std::chrono::steady_clock::now();

    scene.CreateScene("/Users/goksu/Documents/AdvancedRayTracer/hw3_inputs/" + sceneName + ".xml");

    auto loaded = std::chrono::steady_clock::now();
    std::cerr << "Loading took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(loaded - start).count()
              << "ms.\n";

    std::vector<Image> images;

    cnt.store(0);
    std::cerr << "Started rendering the scene...\n";
    auto beginRender = std::chrono::steady_clock::now();

    for (auto& cam : scene.Cameras()){
        images.push_back(cam.Render());
    }
    std::cout << cnt.load() << '\n';
    std::chrono::steady_clock::time_point endRender = std::chrono::steady_clock::now();
    std::cerr << "Rendering took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endRender - beginRender).count()
              << "ms.\n";

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

        cv::imwrite("/Users/goksu/Documents/AdvancedRayTracer/hw3_outputs/" + sceneName + ".png", im);
    }

    return 0;
}