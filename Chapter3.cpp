#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// <summary>
/// Resize and Crop
/// </summary>

void main() {

	string path = "Resources/test.png";
	Mat img = imread(path);
	Mat imgResized, imgCrop;

	//cout << img.size();
	//resize(img, imgResized, Size(640, 480));
	resize(img, imgResized, Size(), 0.5, 0.5);

	Rect roi(200, 100, 300, 300);
	imgCrop = img(roi);

	imshow("Image", img);
	imshow("Image Resized", imgResized);
	imshow("Image Crop", imgCrop);
	waitKey(0);
}