#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	cv::Mat local_image = cv::imread("/Users/goksu/Documents/AdvancedRayTracer/outputs/10/local.png");
    cv::Mat full_image  = cv::imread("/Users/goksu/Documents/AdvancedRayTracer/outputs/10/full.png");
    cv::Mat mask_image  = cv::imread("/Users/goksu/Documents/AdvancedRayTracer/outputs/10/mask.png");
    cv::Mat distant_image = cv::imread("/Users/goksu/Documents/AdvancedRayTracer/outputs/10/distant.png");

    cv::Mat difference(mask_image.size(), CV_16SC3);

    for(int i = 0; i < local_image.rows; i++)
    {
        for (int j = 0; j < local_image.cols; j++)
        {
            auto val1 = (cv::Vec3s)(full_image.at<cv::Vec3b>(j, i));
            auto val2 = (cv::Vec3s)(local_image.at<cv::Vec3b>(j, i));

            difference.at<cv::Vec3s>(j, i) = val1 - val2;
        }
    }

    for(int i = 0; i < distant_image.rows; i++)
    {
        for (int j = 0; j < distant_image.cols; j++)
        {
            if(mask_image.at<cv::Vec3b>(j, i)[0] == 0)
            {
                auto distant = (cv::Vec3s)distant_image.at<cv::Vec3b>(j, i);
                distant += 2 * difference.at<cv::Vec3s>(j, i);
                distant_image.at<cv::Vec3b>(j, i) = (cv::Vec3b)distant;
            }
            else
            {
                distant_image.at<cv::Vec3b>(j, i) = full_image.at<cv::Vec3b>(j, i);
            }
        }
    }

    cv::imshow("name", distant_image);
    cv::waitKey(0);

    cv::imwrite("/Users/goksu/Documents/AdvancedRayTracer/outputs/10/ibl.png", distant_image);

    return 0;
}