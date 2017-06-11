#include <iostream>

#include "Camera.h"
#include "Scene.h"
#include <boost/filesystem/path.hpp>
#include "boost/filesystem/operations.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <chrono>
#include <string>

#include "Shapes/Mesh.h"

Scene scene;

//#define CREATE_MASK

std::string GetFileName(int set, int index);

int main(int argc, char** argv)
{
    int inputset = 10;
    int index = 1;

    if (argc == 3){
        inputset = std::stoi(argv[1]);
        index = std::stoi(argv[2]);
        std::cerr << "hello " <<'\n';
    }
    
    auto sceneName = GetFileName(inputset, index);

    std::cerr << "Started loading the scene " << sceneName << "...\n";
    auto start = std::chrono::steady_clock::now();

    scene.SetPath("/Users/goksu/Documents/AdvancedRayTracer/");
    scene.CreateScene(sceneName);

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
        
        std::string name = "/Users/goksu/Documents/AdvancedRayTracer/outputs/" + std::to_string(inputset) + "/" + std::to_string(index) + "_" + std::to_string(time_elapsed) + "ms.png";
        std::cerr << name << '\n';
        cv::imwrite(name, im);
    }

    return 0;
}

std::string GetFileName(int set, int index)
{
    std::string full_path = "/Users/goksu/Documents/AdvancedRayTracer/inputs/" + std::to_string(set) + "/";
    if (!boost::filesystem::exists(full_path))
    {
        std::cerr << "Could not open " << full_path << '\n';
        abort();
    }
        
    if (!boost::filesystem::is_directory(full_path))
    {
        std::cerr << full_path << " should be a directory.\n";
        abort();
    }
        
    std::vector<std::string> files;
    boost::filesystem::directory_iterator directory_iterator;
    for (boost::filesystem::directory_iterator it(full_path); it != directory_iterator; it++)
    {
        try
        {
            if ( boost::filesystem::is_directory( it->status() ) )
            {
                std::cout << it->path().filename() << " [directory]\n";
            }
            else if ( boost::filesystem::is_regular_file( it->status() ) )
            {
                std::string filename = it->path().filename().string<std::string>();
                files.push_back(full_path + filename);
                //                std::cout << it->path().filename() << "\n";
            }
            else
            {
                std::cout << it->path().filename() << " [other]\n";
            }
        }
        catch ( const std::exception & ex )
        {
            std::cerr << "Problem reading files" << '\n';
            abort();
        }
    }

    return files[index];
}
        

