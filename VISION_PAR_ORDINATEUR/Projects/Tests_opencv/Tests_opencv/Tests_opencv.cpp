// Tests_opencv.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << " Usage: display_image ImageToLoadAndDisplay" << std::endl;
		return -1;
	}
	cv::Mat image;
	image = cv::imread(argv[1], cv::IMREAD_COLOR); // Read the file
	if (image.empty()) // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", image); // Show our image inside it.

	///Exo3
	cv::Mat gray;
	cv::cvtColor(image, gray, CV_BGR2GRAY);
	std::vector < cv::Mat> channels(3);
	cv::split(image, channels);

	///Exo4
	cv::Mat t = gray > 100;
	
	std::vector<cv::Mat> channels_t;
	for (auto c = channels.begin(); c != channels.end(); ++c)
	{
		channels_t.push_back(( *c ) > 100);
	}
	cv::Mat merge_t;
	cv::merge(channels_t, merge_t);

	///Exo5
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);
	std::vector<cv::Mat> hsv_channels(3);
	cv::split(hsv, hsv_channels);
	hsv_channels[1] *= 2;
	cv::merge(hsv_channels, hsv);



	cv::imshow("Image", image);
	cv::imshow("Gray", gray);
	cv::imshow("treshold", t);
	cv::imshow("B", channels[0]);
	cv::imshow("G", channels[1]);
	cv::imshow("R", channels[2]);
	cv::waitKey(0);

	return 0;
}