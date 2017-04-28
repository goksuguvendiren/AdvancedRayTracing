#include <iostream>

#include "Camera.h"
#include "Scene.h"
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>

Scene scene;
//extern std::atomic<std::uint64_t> cnt;

int main(int argc, char** argv)
{
    std::vector<std::string> sceneNames = {"ellipsoids_texture",
                                           "killeroo_diffuse_specular_texture",
                                           "perlin_types",
                                           "simple_texture",
                                           "skybox",
                                           "sphere_texture_blend_bilinear",
                                           "sphere_texture_replace_bilinear",
                                           "sphere_texture_replace_nearest"
    };

    int index = 2;

    if (argc == 2){
        index = std::stoi(argv[1]);
    }

    auto sceneName = sceneNames[index];

    std::cerr << "Started loading the scene " << sceneName << "...\n";
    auto start = std::chrono::steady_clock::now();

    scene.SetPath("/Users/goksu/Documents/AdvancedRayTracer/");
    scene.CreateScene("/Users/goksu/Documents/AdvancedRayTracer/inputs/6/" + sceneName + ".xml");

    auto loaded = std::chrono::steady_clock::now();
    std::cerr << "Loading took "
              <<  std::chrono::duration_cast<std::chrono::milliseconds>(loaded - start).count()
              << "ms.\n";

    std::vector<Image> images;

//    cnt.store(0);
    std::cerr << "Started rendering the scene...\n";
    auto beginRender = std::chrono::steady_clock::now();

    for (auto& cam : scene.Cameras()){
        images.push_back(cam.Render());
    }
//    std::cout << cnt.load() << '\n';
    std::chrono::steady_clock::time_point endRender = std::chrono::steady_clock::now();

    unsigned int time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endRender - beginRender).count();
    std::cerr << "Rendering took "
              << time_elapsed
              << "ms.\n";

    std::cerr << time_elapsed << '\n';

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

        cv::imwrite("/Users/goksu/Documents/AdvancedRayTracer/outputs/6/" + sceneName + "_" + std::to_string(time_elapsed) + "ms.png", im);
    }

    return 0;
}
