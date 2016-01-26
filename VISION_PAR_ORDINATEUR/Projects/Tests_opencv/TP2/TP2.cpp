// TP2.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define DISPLAY 1

cv::Mat convertColor(cv::Mat src, int type)
{
	cv::Mat dst;
	cv::cvtColor(src, dst, type);
	return dst;
}
void displayImage(const std::string name, const cv::Mat img)
{
#if DISPLAY == 1
	cv::imshow(name, img);
#endif
}
void displayImage(const char* name, const cv::Mat img)
{
#if DISPLAY == 1
	cv::imshow(name, img);
#endif
}
void displayImage(const cv::String name, const cv::Mat img)
{
#if DISPLAY == 1
	cv::imshow(name, img);
#endif
}
float max3(float a, float b, float c)
{
	return ( MAX(MAX(a, b), c) );
}
float min3(float a, float b, float c)
{
	return ( MIN(MIN(a, b), c) );
}
struct point2d
{
	int x;
	int y;
};

point2d mousePos;
bool clicked = false;
cv::Mat lol;
int blue;
int green;
int red;
cv::Vec3b  intensity;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
		cv::Mat hsv;
		std::vector<cv::Mat> hsv_channels(3);
		hsv = convertColor(lol, CV_BGR2HSV);
		cv::split(hsv, hsv_channels);
		int hsvMin = 0;
		int hsvMax = 0;
		cv::Vec3b intensityHsv = hsv.at<cv::Vec3b>(cv::Point(x, y));
		int hue = static_cast<int>( intensityHsv[0] );
		int saturation = static_cast<int>( intensityHsv[1] );
		int value = static_cast<int>( intensityHsv[2] );
		hsvMin = (255 + hue - 5) % 255;
		hsvMax = ( 255 + hue + 5 ) % 255;
		cv::Mat mask = ( hsv_channels[0] > hsvMin )&( hsv_channels[0] < hsvMax );
		hsv_channels[1] = hsv_channels[1] & mask;
		cv::merge(hsv_channels, hsv);

		cv::Mat newM = convertColor(hsv, CV_HSV2BGR);
		cv::imshow("New", newM);
	}
	else if (event == cv::EVENT_RBUTTONDOWN)
	{
		std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
	}
	else if (event == cv::EVENT_MBUTTONDOWN)
	{
		std::cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
	}
	else if (event == cv::EVENT_MOUSEMOVE)
	{
		//std::cout << "Mouse move over the window - position (" << x << ", " << y << ")" << std::endl;
		std::cout << "color is : " << lol.row(x).col(y) << "at pos : " << x << ", " << y << std::endl;
		intensity = lol.at<cv::Vec3b>(cv::Point(x, y));
		blue = static_cast<int>( intensity[0] );
		green = static_cast<int>( intensity[1] );
		red = static_cast<int>( intensity[2] );

		std::cout << "blue : " << blue << " green : " << green << " red : " << red << std::endl;
		cv::Mat preview(200, 200, CV_8UC3);
		preview = cv::Scalar(blue, green, red);
		displayImage("preview", preview);

		cv::Mat hsv;
		std::vector<cv::Mat> hsv_channels(3);
		hsv = convertColor(lol, CV_BGR2HSV);
		cv::split(hsv, hsv_channels);
		int hsvMin = 0;
		int hsvMax = 0;
		cv::Vec3b intensityHsv = hsv.at<cv::Vec3b>(cv::Point(x, y));
		int hue = static_cast<int>( intensityHsv[0] );
		int saturation = static_cast<int>( intensityHsv[1] );
		int value = static_cast<int>( intensityHsv[2] );
		hsvMin = ( 255 + hue - 5 ) % 255;
		hsvMax = ( 255 + hue + 5 ) % 255;
		cv::Mat mask = ( hsv_channels[0] > hsvMin )&( hsv_channels[0] < hsvMax );
		hsv_channels[1] = hsv_channels[1] & mask;
		cv::merge(hsv_channels, hsv);

		cv::Mat newM = convertColor(hsv, CV_HSV2BGR);
		cv::imshow("New", newM);

	}
	mousePos.x = x;
	mousePos.y = y;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << " Usage: display_image ImageToLoadAndDisplay" << std::endl;
		return -1;
	}
	cv::Mat image;
	image = cv::imread(argv[1], cv::IMREAD_COLOR); // Read the file, niveau de gris
	if (image.empty()) // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	//displayImage("image", image);

	cv::Mat grey;
	grey = convertColor(image, CV_BGR2GRAY);

	cv::Mat sobelX16;
	cv::Sobel(grey, sobelX16, CV_16S, 1, 0);
	cv::Mat sobelX8;
	sobelX16.convertTo(sobelX8, CV_8U, 0.5, 128);
	//displayImage("SobelX", sobelX8);

	cv::Mat sobelY16;
	cv::Sobel(grey, sobelY16, CV_16S, 1, 1);
	cv::Mat sobelY8;
	sobelY16.convertTo(sobelY8, CV_8U, 0.5, 128);
	//displayImage("SobelY", sobelY8);

	cv::Mat sobelXY16;
	cv::Sobel(grey, sobelXY16, CV_16S, 1, 1);
	cv::Mat sobelXY8;
	sobelXY16.convertTo(sobelXY8, CV_8U, 0.5, 128);
	//displayImage("SobelXY", sobelXY8);

	cv::Mat canny;
	cv::Canny(grey, canny, 50, 100);
	cv::Mat blur;
	cv::blur(grey, blur, cv::Size(5, 5));
	cv::Canny(blur, canny, 50, 100);
	//displayImage("Canny", canny);

	cv::Mat display;
	display = convertColor(grey, CV_GRAY2BGR);
	display.setTo(cv::Scalar(155, 0, 0), canny);
	//displayImage("display mask", display);


	lol = image;
	//lol = convertColor(image, CV_GRAY2BGR);
	displayImage("lolz", lol);
	cv::namedWindow("preview", cv::WINDOW_AUTOSIZE);
	cv::setMouseCallback("lolz", CallBackFunc, NULL);

	//for (int i = 0; i < lol.size().height; ++i)
		//for (int j = 0; j < lol.size().width; ++j)
			//std::cout << lol.row(i).col(j) << std::endl;

	cv::waitKey(0);
	return 0;
}

