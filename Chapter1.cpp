#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


void main() {

	/// Importing images
	/*
	
	string path = "Resources/test.png";
	Mat img = imread(path);  //matrix data type to handle images

	imshow("Image", img);
	waitKey(0);
	
	*/


	///Importing Videos
	/*
	
	string path = "Resources/test_video.mp4";
	VideoCapture cap(path);
	Mat img;

	while (true) {

		cap.read(img);

		imshow("Image", img);
		waitKey(20);
		
	}
	*/

	//Importing Webcam

	VideoCapture cap(0);

	Mat img;

	while (true) {

		cap.read(img);

		imshow("Image", img);
		waitKey(1);

	}


}

