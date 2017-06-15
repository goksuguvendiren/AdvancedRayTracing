#include <iostream>
#include <opencv2/opencv.cpp>

int main()
{
	cv::Mat image = cv::imread("/Users/goksu/Documents/AdvancedRayTracer/outputs/10/local.png");
	std::cerr << "hai" << '\n';
	cv::imshow(image, "image");
	cv::waitKey(0);
	return 0;
}