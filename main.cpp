#include <iostream>

#include "Camera.h"
#include "Scene.h"
#include <opencv2/highgui/highgui.hpp>

Scene scene;

int main()
{
    scene = CreateScene();

    std::vector<Image> images;

    for (auto& cam : scene.Cameras()){
        images.push_back(cam.Render());
    }

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

        cv::imshow("output image", cv::Mat(im));
        cv::waitKey(0);
    }


    return 0;
}