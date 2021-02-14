#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// <summary>
/// Drawing Shapes and Text
/// </summary>

void main() {
	
	//Blank Image

	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));

	circle(img, Point(256, 256), 155, Scalar(0, 200, 123), FILLED);
	
	rectangle(img, Point(100, 100), Point(412, 412), Scalar(203,43,23), 2);
	
	line(img, Point(90, 90), Point(422, 422), Scalar(12,23,34), 5);
	line(img, Point(90, 422), Point(422, 90), Scalar(12, 23, 34), 5);

	putText(img, "Fazli Balkan", Point(100, 255), FONT_HERSHEY_SCRIPT_COMPLEX, 2, Scalar(120,0,120));

	imshow("Image", img);

	waitKey(0);
}