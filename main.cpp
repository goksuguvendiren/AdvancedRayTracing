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
        cv::Mat im = cv::Mat(image.Height(), image.Width(), CV_32FC4);
        for (int i = 0; i < image.Height(); i++){
            for (int j = 0; j < image.Width(); j++){
                im.at<cv::Vec4f>(i, j)[0] = image.at(i, j).R();
                im.at<cv::Vec4f>(i, j)[1] = image.at(i, j).G();
                im.at<cv::Vec4f>(i, j)[2] = image.at(i, j).B();
                im.at<cv::Vec4f>(i, j)[3] = image.at(i, j).A();
            }
        }
        cv::imshow("winname", cv::Mat(im));
        cv::waitKey(0);
    }

    return 0;
}